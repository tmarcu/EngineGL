#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <locale>
#include <vector>
using namespace std;

struct vec3 {
	float x;
	float y;
	float z;
};

struct face {
	unsigned int vertindex[3];
	unsigned int uvindex[3];
	unsigned int normindex[3];
};

static void exerr(string msg)
{
	cout << msg << endl;
}

static void getvertex(char *c, char *n, FILE *f, vector<struct vec3> *vertex)
{
	struct vec3 vert;

	if (fscanf(f, "%f %f %f", &vert.x, &vert.y, &vert.z) <= 0)
		exerr("failed to scan vertices!");
	vertex->push_back(vert);
}

static void getnormal(char *c, char *n, FILE *f, vector<struct vec3> *normal)
{
	struct vec3 vert;

	if (fscanf(f, "%f %f %f", &vert.x, &vert.y, &vert.z) <= 0)
		exerr("failed to scan normals!");
	normal->push_back(vert);
}

static void getface(char *c, char *n, FILE *f, vector<struct face> *face)
{
	struct face temp;

	*c= fgetc(f);
	*c= fgetc(f);
	*n= fgetc(f);
	/* Check for the vertex//normal case */
	if (*n == '/') {
		temp.vertindex[0] = atoi(c);
		*n = fgetc(f);
		if (*n == '/') {
			if (fscanf(f, "%i %i//%i %i//%i",
				&temp.normindex[0],
				&temp.vertindex[1], &temp.normindex[1],
				&temp.vertindex[2], & temp.normindex[2]) <= 0)
					exerr("Failed to read faces!");
		} else { 
			exerr(" VERTEX/TEXTURE FORMAT ");
			exit(EXIT_FAILURE);
		}
	} else if (*n == ' ') {
		/* Handle f v v v format */
		if (fscanf(f, "%i %i %i", &temp.vertindex[0], &temp.vertindex[1], &temp.vertindex[2]) <= 0)
			exerr("Failed to read faces! - expecting: f v v v");
	}


	face->push_back(temp);
}

int main(int argc, char *argv[])
{
	string line;
	FILE *file;
	char curr = 0;
	char next = 0;
	vector<struct vec3> vertices;
	vector<struct vec3> normals;
	vector<struct face> faces;

	file =  fopen(argv[1], "r");
	if (file == NULL) {
		cout << "Unable to open file!\n" << endl;
		exit(EXIT_FAILURE);
	}

	/* Read in data */
	curr = fgetc(file);
	next = curr;
	next = fgetc(file);
	while (curr != EOF) {
		if (curr == 'v' && next == ' ') {
			getvertex(&curr, &next, file, &vertices);
		} else if (curr == 'v' && next == 'n') {
			getnormal(&curr, &next, file, &normals);
		} else if (curr == 'f') {
			getface(&curr, &next, file, &faces);
		}
		curr = next;
		next = fgetc(file);
	}
	cout << vertices.size() << " vertices\n" <<
			normals.size() << " normals\n" <<
			faces.size() << " Triangles (faces)" << endl;
	for (size_t i = 0; i < vertices.size(); i++)
		cout << vertices[i].x << " "<< vertices[i].y << " " << vertices[i].z << endl;
	for (size_t i = 0; i < normals.size(); i++)
		cout << normals[i].x << " " << normals[i].y << " " << normals[i].z << endl;
	for (size_t i = 0; i < faces.size(); i++)
		cout << faces[i].vertindex[0] << "//" << faces[i].normindex[0] << endl;
	for (size_t i = 0; i < faces.size(); i++)
		cout << faces[i].vertindex[0] << " " << faces[i].vertindex[1] << " " << faces[i].vertindex[2] << endl;

	fclose(file);

	return 0;
}
