#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <locale>
#include <vector>
#include <string.h>
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
	char line[128]; /* Should accomodate very big models... */

	fgets(line, 128, f);
	if (sscanf(line, "%i %i %i", &temp.vertindex[0], &temp.vertindex[1], &temp.vertindex[2]) <= 0) {
		/* f v1 v2 v3 */
	} else if (sscanf(line, "%i/%i %i/%i %i/%i",
				&temp.vertindex[0],&temp.uvindex[0],
				&temp.vertindex[1], &temp.uvindex[1],
				&temp.vertindex[2], & temp.uvindex[2]) == 6) {
		/* f v1/vt1 v2/vt2 v3/vt3 */
	} else if (sscanf(line, "%i//%i %i//%i %i//%i",
				&temp.vertindex[0],&temp.normindex[0],
				&temp.vertindex[1], &temp.normindex[1],
				&temp.vertindex[2], & temp.normindex[2]) == 6) {
		/* f v1//vn1 v2//vn2 v3//vn3 */
	} else if (sscanf(line, "%i/%i/%i %i/%i/%i %i/%i/%i",&temp.vertindex[0],
				&temp.uvindex[0],&temp.normindex[0],
				&temp.vertindex[1],
				&temp.uvindex[1],
				&temp.normindex[1],
				&temp.vertindex[2],
				&temp.uvindex[2],
				&temp.normindex[2]) == 9) {
		/* f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 */
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
/*	for (size_t i = 0; i < vertices.size(); i++)
		cout << vertices[i].x << " "<< vertices[i].y << " " << vertices[i].z << endl;
	for (size_t i = 0; i < normals.size(); i++)
		cout << normals[i].x << " " << normals[i].y << " " << normals[i].z << endl;*/
	for (size_t i = 0; i < faces.size(); i++)
		cout << faces[i].vertindex[0] << "//" << faces[i].uvindex[0] << endl;
	//for (size_t i = 0; i < faces.size(); i++)
	//	cout << faces[i].vertindex[0] << " " << faces[i].vertindex[1] << " " << faces[i].vertindex[2] << endl;
	//for (size_t i = 0; i < faces.size(); i++)
	//	cout << faces[i].vertindex[0] << " " << faces[i].uvindex[0] << " " << faces[i].normindex[0] << "|" << faces[i].vertindex[1] << " " << faces[i].uvindex[1] << " " << faces[i].normindex[1] << "|" << faces[i].vertindex[2] << " " << faces[i].uvindex[2] << " " << faces[i].normindex[2]<< endl;
	fclose(file);

	return 0;
}

