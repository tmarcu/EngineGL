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

int main(int argc, char *argv[])
{
	string line;
	FILE *file;
	char c = 0;
	char next = 0;
	struct vec3 vertex;
	struct face temp;
	vector<struct vec3> vertices;
	vector<struct vec3> normals;
	vector<struct face> faces;

	file =  fopen(argv[1], "r");
	if (file == NULL) {
		cout << "Unable to open file!\n" << endl;
		exit(EXIT_FAILURE);
	}

	/* Read in data */
	c = fgetc(file);
	next = c;
	next = fgetc(file);
	while (c != EOF) {
		if (c == 'v' && next == ' ') {
			if (fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z) <= 0)
				exerr("failed to scan vertices!");
			vertices.push_back(vertex);
		} else if (c == 'v' && next == 'n') {
			if (fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z) <= 0)
				exerr("failed to scan normals!");
			normals.push_back(vertex);
		} else if (c == 'f') {
			c = fgetc(file);
			c = fgetc(file);
			next = fgetc(file);
			/* Check for the vertex//normal case */
			if (next == '/') {
				temp.vertindex[0] = atoi(&c);
				next = fgetc(file);
				if (next == '/') {
					if (fscanf(file, "%i %i//%i %i//%i",
						&temp.normindex[0],
						&temp.vertindex[1], &temp.normindex[1],
						&temp.vertindex[2], & temp.normindex[2]) <= 0)
							exerr("Failed to read faces!");
					}
			}
			faces.push_back(temp);
		}
		c = next;
		next = fgetc(file);
	}
	cout << vertices.size() << " vertices\n" <<
			normals.size() << " normals\n" <<
			faces.size() << " faces" << endl;
	for (size_t i = 0; i < vertices.size(); i++)
		cout << vertices[i].x << " "<< vertices[i].y << " " << vertices[i].z << endl;

	for (size_t i = 0; i < normals.size(); i++)
		cout << normals[i].x << " " << normals[i].y << " " << normals[i].z << endl;
	for (size_t i = 0; i < faces.size(); i++)
		cout << faces[i].vertindex[0] << "//" << faces[i].normindex[0] << endl;
	return 0;
}
