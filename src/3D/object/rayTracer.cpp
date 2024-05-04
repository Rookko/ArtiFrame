#include "rayTracer.h"
#include <cmath>   
#include <iostream> 
#include <sstream>  
#include <vector>
#include <random>
#include <chrono>
#include "ofxAssimpModelLoader.h"
#include "curve.h"
#include "cylinderPrimitive.h"
#include "loadedFile.h"
#include "object.h"
#include "operation.h"
#include "scene.h"
#include "spherePrimitive.h"
#include "surface.h"
#include "transformation.h"
#include "light.h"
#include <omp.h>
#include <atomic> // Include for atomic variable


const int max_depth = 5;
const double camera_fov = 0.5135;
constexpr double gamma_correction = 1 / 2.2;

int image_width = 0;
int image_height = 0;
int ray_per_pixel = 0;
int pixel_count = 0;

enum class SurfaceType { diffuse, specular, refraction };

double clamp(double x) {
    return x < 0 ? 0 : x > 1 ? 1 : x;
}

struct Vector {
    double x, y, z;

    constexpr Vector() : x(0.0), y(0.0), z(0.0) {}
    constexpr Vector(double x) : x(x), y(0.0), z(0.0) {}
    constexpr Vector(double x, double y) : x(x), y(y), z(0.0) {}
    constexpr Vector(double x, double y, double z) : x(x), y(y), z(z) {}

    double dot(const Vector& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector cross(const Vector& v) const {
        return Vector(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x);
    }

    Vector multiply(const Vector& v) const {
        return Vector(
            x * v.x,
            y * v.y,
            z * v.z);
    }

    Vector operator*(double s) const {
        return Vector(
            x * s,
            y * s,
            z * s);
    }

    Vector operator+(const Vector& v) const {
        return Vector(
            x + v.x,
            y + v.y,
            z + v.z);
    }

    Vector operator-(const Vector& v) const {
        return Vector(
            x - v.x,
            y - v.y,
            z - v.z);
    }

    Vector& operator+=(const Vector& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector& normalize() {
        double len = length();
        if (len > 0) { // to avoid division by zero
            double invLen = 1.0 / len;
            x *= invLen;
            y *= invLen;
            z *= invLen;
        }
        return *this;
    }

    double length() const {
        return sqrt(x * x + y * y + z * z);
    }
};


struct Ray {
    Vector origin;
    Vector direction;
    Ray(Vector o, Vector d) : origin(o), direction(d) {}
};

struct Intersection {
    double distance;
    Vector normale;
    Intersection() : distance(0), normale(Vector()) {}
    Intersection(double d, Vector n) : distance(d), normale(n) {}
};

struct Element {
    Vector position;
    Vector emission;
    Vector color;
    SurfaceType material;

    Element(Vector p, Vector e, Vector c, SurfaceType m)
        : position(p), emission(e), color(c), material(m) {}

    virtual Intersection intersect(const Ray& ray) const = 0;
};

bool rayIntersectsBox(const Ray& ray, const ofPoint& min, const ofPoint& max) {
    // Calculate the minimum and maximum points of the box
    Vector boxMin = Vector(std::min(min.x, max.x), std::min(min.y, max.y), std::min(min.z, max.z));
    Vector boxMax = Vector(std::max(min.x, max.x), std::max(min.y, max.y), std::max(min.z, max.z));

    // Calculate the t values for each plane
    double t_x_min = (boxMin.x - ray.origin.x) / ray.direction.x;
    double t_x_max = (boxMax.x - ray.origin.x) / ray.direction.x;
    double t_y_min = (boxMin.y - ray.origin.y) / ray.direction.y;
    double t_y_max = (boxMax.y - ray.origin.y) / ray.direction.y;
    double t_z_min = (boxMin.z - ray.origin.z) / ray.direction.z;
    double t_z_max = (boxMax.z - ray.origin.z) / ray.direction.z;

    // Determine the maximum and minimum t values
    double t_max = std::min(std::min(t_x_max, t_y_max), t_z_max);
    double t_min = std::max(std::max(t_x_min, t_y_min), t_z_min);

    // Check if the ray intersects the box
    return t_max >= t_min;
}

struct Model : Element {
    ofMesh mesh;
    ofPoint min;
    ofPoint max;
    glm::mat4 mat;
    vector<Vector> vertex;
    vector<Vector> edge1;
    vector<Vector> edge2;

    Model(ofMesh ofmesh, ofPoint mini, ofPoint maxi, glm::mat4 mat, Vector p, Vector e, Vector c, SurfaceType m)
        : Element(p, e, c, m) {
        mesh = ofmesh;
        min = (mini * 20).operator+(ofPoint(position.x, position.y, position.z));
        max = (maxi * 20).operator+(ofPoint(position.x, position.y, position.z));
        for (ofMeshFace face : mesh.getUniqueFaces()) {
            Vector v0 = Vector(face.getVertex(0).x, face.getVertex(0).y, face.getVertex(0).z);
            Vector v1 = Vector(face.getVertex(1).x, face.getVertex(1).y, face.getVertex(1).z);
            Vector v2 = Vector(face.getVertex(2).x, face.getVertex(2).y, face.getVertex(2).z);
            v0 = (v0 * 20) + position;
            v1 = (v1 * 20) + position;
            v2 = (v2 * 20) + position;
            vertex.push_back(v0);
            vertex.push_back(v1);
            vertex.push_back(v2);
            Vector e1 = v1 - v0;
            edge1.push_back(e1);
            Vector e2 = v2 - v0;
            edge2.push_back(e2);
        }
    };

    Intersection intersect(const Ray& ray) const {
        if (rayIntersectsBox(ray, min, max))
            return Intersection();

        Vector normale = Vector();
        double minDistance = std::numeric_limits<double>::infinity();
        int index = 0;
        for (ofMeshFace face : mesh.getUniqueFaces()) {
            Vector v0 = vertex[index * 3];
            Vector v1 = vertex[(index * 3) + 1];
            Vector v2 = vertex[(index * 3) + 2];
            Vector e1 = edge1[index];
            Vector e2 = edge2[index];

            index++;
            Vector p = ray.direction.cross(e2);
            double det = e1.dot(p);
            if (std::fabs(det) < std::numeric_limits<double>::epsilon()) {
                // Ray is parallel to triangle, no intersection
                continue;
            }
            Vector t = ray.origin - v0;
            double u = t.dot(p) / det;
            if (u < 0.0 || u > 1.0) {
                // Intersection point is outside the triangle
                continue;
            }
            Vector q = t.cross(e1);
            double v = ray.direction.dot(q) / det;
            if (v < 0.0 || u + v > 1.0) {
                // Intersection point is outside the triangle
                continue;
            }
            double distance = e2.dot(q) / det;
            if (distance < minDistance) {
                minDistance = distance;
                normale = (ray.origin + ray.direction * distance - position).normalize();
            }
        }
        return Intersection(minDistance, normale);
    }
};

struct Sphere : Element {
    double radius;
    Sphere(double r, Vector p, Vector e, Vector c, SurfaceType m)
        : Element(p, e, c, m), radius(r) {};

    Intersection intersect(const Ray& ray) const {
        double distance;
        double epsilon = 1e-4;
        double t;
        Vector delta = position - ray.origin;
        double a = delta.dot(delta);
        double b = delta.dot(ray.direction);
        double c = radius * radius;
        double discriminant = b * b - a + c;

        if (discriminant < 0) {
            return Intersection();
        }

        discriminant = sqrt(discriminant);
        t = b - discriminant;

        if (t > epsilon)
            distance = t;
        else {
            t = b + discriminant;

            if (t > epsilon)
                distance = t;
            else
                distance = 0;
        }

        return Intersection(distance, (ray.origin + ray.direction * distance - position).normalize());
    }
};

struct Cube : Element {
    double size;

    Cube(double s, Vector p, Vector e, Vector c, SurfaceType m)
        : Element(p, e, c, m), size(s) {};

    Intersection intersect(const Ray& ray) const {
        double tMin = (position.x - size / 2 - ray.origin.x) / ray.direction.x;
        double tMax = (position.x + size / 2 - ray.origin.x) / ray.direction.x;

        if (tMin > tMax) std::swap(tMin, tMax);

        double tYMin = (position.y - size / 2 - ray.origin.y) / ray.direction.y;
        double tYMax = (position.y + size / 2 - ray.origin.y) / ray.direction.y;

        if (tYMin > tYMax) std::swap(tYMin, tYMax);

        if ((tMin > tYMax) || (tYMin > tMax)) return Intersection();

        if (tYMin > tMin) tMin = tYMin;

        if (tYMax < tMax) tMax = tYMax;

        double tZMin = (position.z - size / 2 - ray.origin.z) / ray.direction.z;
        double tZMax = (position.z + size / 2 - ray.origin.z) / ray.direction.z;

        if (tZMin > tZMax) std::swap(tZMin, tZMax);

        if ((tMin > tZMax) || (tZMin > tMax)) return Intersection();

        if (tZMin > tMin) tMin = tZMin;

        if (tZMax < tMax) tMax = tZMax;

        if (tMin < 0) return Intersection();

        float distance = tMin;
        return Intersection(distance, (ray.origin + ray.direction * distance - position).normalize());
    }
};

struct Camera
{
    Vector position;
    Vector orientation;
    Vector target;

    Vector axis_x;
    Vector axis_y;
    Vector axis_z;

    Vector up;

    float viewport_width;
    float viewport_height;

    float fov;

    Camera(Vector p, Vector o) : position(p), orientation(o) {}

    // fonction qui permet de calibrer la cam�ra en fonction la valeur courante de ses attributs
    void calibrate()
    {
        axis_z = orientation;
        axis_x = Vector(viewport_width * fov / viewport_height);
        axis_y = axis_x.cross(axis_z).normalize() * fov;
    }
};

struct Image {
    int width;  // largeur de l'image
    int height; // hauteur de l'image
    int count;  // nombre de pixels
    int index;  // index d'un des pixels

    double size; // poids en m�moire (en m�gaoctets)

    Vector* pixel;

    Image() : width(1), height(1) {}
    Image(int w) : width(w), height(w) {}
    Image(int w, int h) : width(w), height(h) {}

    void resize(int w, int h) {
        if (w > 0 && h > 0) {
            width = w;
            height = h;
            count = width * height;
            size = sizeof(Vector) * count / pow(1024.0, 2);
            pixel = new Vector[count];
        }
    }

    ~Image() {
        if (pixel != nullptr)
            delete[] pixel;
    }
};

std::vector<Element*> scene;
Image image;
std::random_device rd;
std::mt19937 rng{ rd() };
std::uniform_real_distribution<double> random01{ 0.0, 1.0 };

int format_color_component(double value) {
    value = pow(clamp(value), gamma_correction) * 255 + 0.5;
    return static_cast<int>(value);
}

Vector compute_radiance(const Ray& ray, int depth) {

    if (depth == 100) {
        return Vector(0, 0, 0);
    }

    // valeur de la radiance
    Vector radiance;

    // distance de l'intersection
    double distance;

    // identifiant de la g�om�trie en intersection
    int id = 0;

    // valider s'il n'y a pas intersection
    Intersection intersection;
    Vector normale;
    double infinity = distance = 1e20;
    int nb = static_cast<int>(scene.size());
    for (int index = 0; index < nb; ++index) {
        intersection = scene[index]->intersect(ray);
        if (intersection.distance && intersection.distance < distance) {
            distance = intersection.distance;
            normale = intersection.normale;
            id = index;
        }
    }
    if (distance >= infinity)
        return Vector();

    // r�f�rence sur une g�om�trie en intersection avec un rayon
    const Element& obj = *scene[id];

    // calculer les coordonn�es du point d'intersection
    Vector x = ray.origin + ray.direction * distance;

    // calculer la normale au point d'intersection
    Vector n = normale;

    // ajustement de la direction de la normale
    Vector na = n.dot(ray.direction) < 0 ? n : n * -1;

    // isoler la composante de couleur la plus puissante
    Vector f = obj.color;
    double threshold = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;

    // valider si la limite du nombre de r�cursions est atteinte
    if (++depth > max_depth)
    {
        // test de probabilit�
        if (random01(rng) < threshold)
            f = f * (1 / threshold);
        else
            return obj.emission;
    }

    if (obj.material == SurfaceType::diffuse)
    {
        // mat�riau avec r�flexion diffuse

        double r1 = 2 * PI * random01(rng);
        double r2 = random01(rng);
        double r2s = sqrt(r2);

        Vector w = na;
        Vector u = ((fabs(w.x) > 0.1 ? Vector(0, 1) : Vector(1)).cross(w)).normalize();
        Vector v = w.cross(u);
        Vector d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();

        radiance = obj.emission + f.multiply(compute_radiance(Ray(x, d), depth));

        return radiance;
    }
    else if (obj.material == SurfaceType::specular)
    {
        // mat�riau avec r�flexion sp�culaire

        radiance = obj.emission + f.multiply(compute_radiance(Ray(x, ray.direction - n * 2.0 * n.dot(ray.direction)), depth));

        return radiance;
    }
    else if (obj.material == SurfaceType::refraction)
    {
        // mat�riau avec r�flexion r�fraction

        Ray reflection_ray(x, ray.direction - n * 2.0 * n.dot(ray.direction));

        bool into = n.dot(na) > 0;

        double ior = 1.5; // indice de r�fraction du verre
        double nc = 1.0;
        double nt = ior;
        double nnt = into ? nc / nt : nt / nc;
        double ddn = ray.direction.dot(na);
        double cos2t;

        if ((cos2t = 1.0 - nnt * nnt * (1.0 - ddn * ddn)) < 0.0)
        {
            radiance = obj.emission + f.multiply(compute_radiance(reflection_ray, depth));

            return radiance;
        }

        Vector tdir = (ray.direction * nnt - n * ((into ? 1.0 : -1.0) * (ddn * nnt + sqrt(cos2t)))).normalize();

        // effet de fresnel
        double a = nt - nc;
        double b = nt + nc;
        double r0 = a * a / (b * b);
        double c = 1.0 - (into ? -ddn : tdir.dot(n));
        double re = r0 + (1.0 - r0) * c * c * c * c * c;
        double tr = 1 - re;
        double p = 0.25 + 0.5 * re;
        double rp = re / p;
        double tp = tr / (1.0 - p);

        radiance = obj.emission + f.multiply(depth > 2 ? (random01(rng) < p ?
            compute_radiance(reflection_ray, depth) * rp : compute_radiance(Ray(x, tdir), depth) * tp) :
            compute_radiance(reflection_ray, depth) * re + compute_radiance(Ray(x, tdir), depth) * tr);

        return radiance;
    }
    else
    {
        return radiance;
    }
}

void save_image_file(int width, int height, int ray_per_pixel, const Vector* pixel)
{
    // nom du fichier image de type .ppm (portable pixmap)
    std::stringstream ss;
    ss << "image" << width << "x" << height << "_" << ray_per_pixel << ".ppm";
    std::string filename = ss.str();

    // d�claration et ouverture du fichier en mode �criture
    std::ofstream file;
    file.open(filename, std::ios::out);

    // ent�te du ficher pour une image avec un espace de couleur RGB 24 bits (P3 pour pixmap)
    file << "P3\n";

    // largeur et hauteur de l'image sur la seconde ligne de l'ent�te
    file << width << ' ' << height << '\n';

    // valeur maximale de l'espace de couleur sur la troisi�me ligne de l'ent�te
    file << "255\n";

    // �criture des pixels dans le fichier image
    for (int index = 0; index < width * height; ++index)
    {
        // �crire la couleur du pixel dans le fichier image
        file << static_cast<std::uint32_t>(format_color_component(pixel[index].x)) << " ";
        file << static_cast<std::uint32_t>(format_color_component(pixel[index].y)) << " ";
        file << static_cast<std::uint32_t>(format_color_component(pixel[index].z)) << " ";
    }

    // fermeture du fichier
    file.close();
}

void render(Camera camera)
{
    std::cout << "render start" << std::endl;

    int x, y, s, sx, sy;
    std::atomic<int> completedLines(0);
    int totalLines = image_height;
    Vector radiance, distance;

    // Détermination du nombre de threads qui seront utilisés
    int nThreads;
#pragma omp parallel
    {
#pragma omp single
        nThreads = omp_get_num_threads();
    }

    // Parallélisation avec OpenMP
#pragma omp parallel for private(x, s, sx, sy, radiance, distance) schedule(dynamic)
    for (y = 0; y < image_height; ++y) {
        for (x = 0; x < image_width; ++x) {
            int index = (image_height - y - 1) * image_width + x;
            for (sy = 0; sy < 2; ++sy) {
                for (sx = 0; sx < 2; ++sx) {
                    radiance = Vector();
                    for (s = 0; s < ray_per_pixel; ++s) {
                        double r1 = 2.0 * random01(rng);
                        double dx = r1 < 1.0 ? sqrt(r1) - 1.0 : 1.0 - sqrt(2.0 - r1);
                        double r2 = 2.0 * random01(rng);
                        double dy = r2 < 1.0 ? sqrt(r2) - 1.0 : 1.0 - sqrt(2.0 - r2);
                        Vector distance = camera.axis_x * (((sx + 0.5 + dx) / 2.0 + x) / image_width - 0.5) +
                            camera.axis_y * (((sy + 0.5 + dy) / 2.0 + y) / image_height - 0.5) + camera.axis_z;
                        radiance += compute_radiance(Ray(camera.position + distance * 140, distance.normalize()), 0) * (1.0 / ray_per_pixel);
                    }
                    image.pixel[index] += Vector(clamp(radiance.x), clamp(radiance.y), clamp(radiance.z)) * 0.25;
                }
            }
        }
        ++completedLines;
        if (omp_get_thread_num() == 0) { // Affichage uniquement par le thread principal
            std::cout << "\rRendering progress: " << static_cast<int>(100.0 * completedLines / totalLines) << "%";
        }
    }

    std::cout << "\nRendering completed. Total progress: 100%\n";
}


void RayTracer::run(ofCamera* cam, Scene* sc) {

    std::cout << "Raytracer start" << std::endl;

    image_width = 300;
    image_height = 300;
    ray_per_pixel = 50;

    constexpr double anchor = 1e5;
    constexpr double wall_radius = anchor;

    scene.clear();

    // Walls
    scene.push_back(&Sphere(wall_radius, Vector(0, -anchor - 50, 0), Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse));    // plancher
    scene.push_back(&Sphere(wall_radius, Vector(0, anchor + 50, 0), Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse));    // plafond
    scene.push_back(&Sphere(wall_radius, Vector(-anchor + 50, 0, 0), Vector(), Vector(0.15, 0.15, 0.75), SurfaceType::diffuse));    // mur droit
    scene.push_back(&Sphere(wall_radius, Vector(anchor - 50, 0, 0), Vector(), Vector(0.75, 0.1, 0.75), SurfaceType::diffuse));  // Mur gauche en mauve
    scene.push_back(&Sphere(wall_radius, Vector(0, 0, -anchor - 50), Vector(), Vector(0.9, 0.3, 0.1), SurfaceType::diffuse));   // Mur arrière en orange brûlé
    scene.push_back(&Sphere(wall_radius, Vector(0, 0, anchor + 250), Vector(), Vector(0.15, 0.75, 0.15), SurfaceType::diffuse));    // Mur avant en vert


    scene.push_back(&Sphere(13, Vector(30, -35, 120), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::refraction)); // Sphere vitre devant droit
    scene.push_back(&Cube(15, Vector(-35, -40, 100), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::refraction)); // Cube de vitre devant gauche
    scene.push_back(&Cube(35, Vector(25, 25, -13), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::specular)); // Cube miroir fond droit
    scene.push_back(&Cube(20, Vector(0, -15, 50), Vector(), Vector(1.0, 0.1, 0.1), SurfaceType::diffuse)); // Cube rouge milieu
    scene.push_back(&Sphere(20, Vector(0, 60, 20), Vector(15, 15, 15), Vector(1.0, 1.0, 1.0), SurfaceType::diffuse)); // Lumiere plafond
    scene.push_back(&Sphere(20, Vector(0, -25, 250), Vector(15, 15, 15), Vector(1.0, 1.0, 1.0), SurfaceType::diffuse)); // Lumiere camera devant
    // Ajouter une sphère miroir fond gauche
    scene.push_back(new Sphere(
        30,                           // rayon de la sphère
        Vector(-25, -25, -10),          // position de la sphère
        Vector(0, 0, 0),              // émission (lumière), ici aucune émission
        Vector(1, 1, 1),              // couleur (blanc, pour refléter comme un miroir)
        SurfaceType::specular         // type de surface spéculaire pour agir comme un miroir
    ));

    for (Object* obj : sc->objects) {
        if (dynamic_cast<LoadedFile*>(obj) != nullptr) {
            LoadedFile* loadedFile = dynamic_cast<LoadedFile*>(obj);

            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, glm::vec3(loadedFile->translationX, loadedFile->translationY, loadedFile->translationZ));
            if (loadedFile->rotationX && loadedFile->rotationY && loadedFile->rotationZ)
                trans = glm::rotate(trans, glm::radians(1.0f), glm::vec3(loadedFile->rotationX, loadedFile->rotationY, loadedFile->rotationZ));
            trans = glm::scale(trans, glm::vec3(loadedFile->scaleX, loadedFile->scaleY, loadedFile->scaleZ));

            Model* model = new Model(loadedFile->model->getMesh(0),
                loadedFile->model->getSceneMin(), loadedFile->model->getSceneMax(), trans,
                Vector(0, 0, 50), Vector(), Vector(1, 1, 1), SurfaceType::diffuse);
            scene.push_back(model);
        }

    }

    // allocation de la m�moire de l'image en fonction des param�tres du programme
    image.resize(image_width, image_height);

    std::cout << "image resize to " << image.width << "x" << image.height << " (" << image.count << " pixels, " << image.size << " MB)" << std::endl;

    Vector camera_orientation(Vector(0, -0.042612, -1).normalize());
    constexpr Vector camera_position(0, 0, 300);

    Camera camera = Camera(camera_position, camera_orientation);
    camera.viewport_width = image.width;
    camera.viewport_height = image.height;
    camera.fov = cam->getFov() / 114;
    camera.calibrate();

    auto t1 = std::chrono::high_resolution_clock::now();

    render(camera);

    auto t2 = std::chrono::high_resolution_clock::now();

    auto render_time = t2 - t1;

    std::cout << "render time : " << std::chrono::duration_cast<std::chrono::seconds>(render_time).count() << "s" << std::endl;

    save_image_file(image.width, image.height, ray_per_pixel, image.pixel);

    std::cout << "Raytracer task is done" << std::endl;
}