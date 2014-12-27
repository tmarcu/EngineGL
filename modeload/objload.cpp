#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <vector>
using namespace std;

struct vec3 {
	float x;
	float y;
	float z;
};

static void exerr(string msg)
{
	cout << msg << endl;
}

int main(int argc, char *argv[])
{
	string line;
	FILE *file;
	char buff[64];
	char c = 0;
	char next = 0;
	struct vec3 vertex;
	vector<struct vec3> vertices;
	vector<struct vec3> normals;
	vector<struct vec3> faces;

	file =  fopen(argv[1], "r");
	if (file == NULL) {
		cout << "Unable to open file!\n" << endl;
		exit(EXIT_FAILURE);
	}

	/* Skip header */
	for (int i = 0; i < 5; i++) {
		if (fgets(buff, 64, file) == NULL)
			exerr("fgets() failed!");
	}

	/* Read in data */
	c = getc(file);
	while (c != EOF) {
		next = getc(file);
		if (c == 'v' && next == ' ') {
			if (fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z) <= 0)
				exerr("failed to scan vertices!");
			vertices.push_back(vertex);
		} else if (next == 'n') {
			if (fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z) <= 0)
				exerr("failed to scan normals!");
			normals.push_back(vertex);
		}

	    if (c == 'f') {
			if (fscanf(file, "%f//%f", &vertex.x, &vertex.z) <= 0)
				exerr("Failed to read faces!");
			faces.push_back(vertex);
		}
		c = next;
		next = getc(file);
	}

	for (size_t i = 0; i < vertices.size(); i++)
		cout << vertices[i].x << " "<< vertices[i].y << " " << vertices[i].z << endl;

	return 0;
}
