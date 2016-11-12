#include "Terrain.h"

using namespace std;

const float Terrain::TERRAIN_SIZE = 301.43f;    // Set so that the fences fit better
const float Terrain::TERRAIN_MAX_HEIGHT = 10.0f;

#define M_PI 3.14159265

// Constructor accepts a model defining vertex, colour and index data for this Terrain.
Terrain::Terrain(Model* model, std::vector<GLuint> textures, Image heightMap) : 
    Entity(model), 
    textures(textures), 
    heightMap(heightMap) {
}

Terrain* Terrain::loadTerrain(std::vector<std::string> images, std::string heightMapFile){
    Image heightMap = Loader::getLoader()->loadImage(heightMapFile);
    Model* model = Terrain::generateTerrainModel(heightMap);
    std::vector<GLuint> textures;
    for(size_t i =0; i < images.size(); i++){
        textures.push_back(Loader::getLoader()->loadTexture(images[i]));
    }

    return new Terrain(model, textures, heightMap);
}

Model* Terrain::generateTerrainModel(Image heightMap){
    std::vector<float> vertices;
    std::vector<float> textureCoords;
    std::vector<unsigned int> indices;
    const int TERRAIN_NUM_VERTS = heightMap.width;

    for(int z_off = 0; z_off < TERRAIN_NUM_VERTS; z_off++){
        for(int x_off = 0; x_off < TERRAIN_NUM_VERTS; x_off++){
            glm::vec3 colour = heightMap.getPixel(x_off, z_off);
            float height = (colour.r + colour.g + colour.b)/3*TERRAIN_MAX_HEIGHT;

            vertices.push_back((float)x_off / ((float)TERRAIN_NUM_VERTS - 1) * TERRAIN_SIZE);
            vertices.push_back(height);
            vertices.push_back((float)z_off / ((float)TERRAIN_NUM_VERTS - 1) * TERRAIN_SIZE);
            textureCoords.push_back((float) x_off /((float)TERRAIN_NUM_VERTS - 1));
            textureCoords.push_back((float) z_off /((float)TERRAIN_NUM_VERTS - 1));
        }
    }

    for(int z_off=0; z_off < TERRAIN_NUM_VERTS - 1; z_off++){
        for(int x_off = 0; x_off < TERRAIN_NUM_VERTS - 1; x_off++){
            int topLeft = (z_off * TERRAIN_NUM_VERTS) + x_off;
            int topRight = topLeft + 1;
            int bottomLeft = ((z_off+1) * TERRAIN_NUM_VERTS) + x_off;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    std::vector<float> normals =  Loader::getLoader()->generateNormals(vertices, indices);

    GLuint vao = Loader::getLoader()->loadVAO(vertices, indices, textureCoords, normals);
    GLuint tex = Loader::getLoader()->loadDefaultTexture();

    ModelComponent component(vao, indices.size(), tex);
    Model* model = new Model();
    model->addRange(vertices);
    model->addModelComponent(component);

    return model;
}

bool Terrain::isOnTerrain(float x, float z){
    int x_int = convertCoordinate(x);
    int z_int = convertCoordinate(z);

    // Bring in boundaries slightly from absolute edge and do check.
    if(x_int < 4
        || x_int >= heightMap.width-4
        || z_int < 4
        || z_int >= heightMap.width-4){
        return false;
    }

    if(getHeight(x,z) < 0.0f){
        return false;
    }

    return true;
}


GLuint Terrain::getVaoID(){
    return model->getModelComponents()->at(0).getVaoID();
}

int Terrain::getIndexCount(){
    return model->getModelComponents()->at(0).getIndexCount();
}

GLuint Terrain::getTextureID(int i){
    return textures[i];
}
float Terrain::getHeight(float x, float z){
    int x_int = convertCoordinate(x);
    int z_int = convertCoordinate(z);
    glm::vec3 pixel = heightMap.getPixel(x_int,z_int);

    return (pixel.r + pixel.g + pixel.b)/3*TERRAIN_MAX_HEIGHT;
}

// Assumes already translated coordinates
float Terrain::getHeight(int x_int, int z_int){
    glm::vec3 pixel = heightMap.getPixel(x_int,z_int);

    return (pixel.r + pixel.g + pixel.b)/3*TERRAIN_MAX_HEIGHT;
}

int Terrain::convertCoordinate(float coord){
    return int((heightMap.width/2) + coord*heightMap.width/TERRAIN_SIZE);
}

glm::vec3 Terrain::getPositionFromPixel(int x, int y){
    float x_flt = ((float)(x - heightMap.width/2))/heightMap.width * TERRAIN_SIZE;
    float z_flt = ((float)(y - heightMap.height/2))/heightMap.height * TERRAIN_SIZE;

    return glm::vec3(x_flt, 0.0f, z_flt);
}

float Terrain::getAngleX(float x, float y, float rotation){
    return getAngle(x,y,rotation, 0.0f);
}

float Terrain::getAngleZ(float x, float y, float rotation){
    return getAngle(x,y,rotation, (float)M_PI/2);
}

// Returns the angle that should be applied for the car given its direction and heading to conform to terrain.
float Terrain::getAngle(float x, float z, float rotation, float offset){
    int x_cur = convertCoordinate(x);
    int z_cur = convertCoordinate(z);

    int x_nxt = 0;
    int z_nxt = 0;
    rotation += (float)M_PI/8; // apply small rotation to offset
    rotation -= offset; // Allows to test both front and next to car
    if(rotation < 0) rotation += (float)M_PI *2;
    if(rotation > 0 && rotation <= (float)M_PI/4){ // North
        z_nxt = -1;
    } else if(rotation > (float)M_PI/4 && rotation <= (float)M_PI/2){ // North west
        x_nxt = -1;
        z_nxt = -1;
    }else if(rotation > (float)M_PI/2 && rotation <= (float)3*M_PI/4){ // West
        x_nxt = -1;
    }else if(rotation > (float)3*M_PI/4 && rotation <= (float)M_PI){    // South west
        x_nxt = -1;
        z_nxt = +1;
    }else if(rotation > (float)M_PI && rotation <= (float)5*M_PI/4){    // South
        z_nxt = +1;
    }else if(rotation > (float)5*M_PI/4 && rotation <= (float)3*M_PI/2){ // South east
        x_nxt = +1;
        z_nxt = +1;
    }else if(rotation > (float)3*M_PI/2 && rotation <= (float)7*M_PI/4){    // East
        x_nxt = +1;
    }else if(rotation > (float)7*M_PI/4 && rotation <= (float)2*M_PI){  // North East
        x_nxt = +1;
        z_nxt = -1;
    }

    float h_cur = getHeight(x_cur, z_cur);
    // Looks 5 units ahead for a next height
    float h_nxt = getHeight(x_cur + x_nxt*5, z_cur+ z_nxt*5);
        
    // However, if that is negative (meaning off map), it tightens until either 0 or a good reading is received
    const int LOOK_AHEAD = 5;
    for (int i = LOOK_AHEAD; i > 0 ; --i){
        if(h_nxt > 0.0) break;
        h_nxt = getHeight(x_cur + x_nxt*i, z_cur+ z_nxt*i);
    }

    return glm::atan((h_nxt - h_cur)/((float)LOOK_AHEAD * TERRAIN_SIZE / heightMap.width));
}





