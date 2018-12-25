#include <gl/glew.h>
#include <gl/freeglut.h> 
#include <SOIL.h>
#include <iostream> 
#include <vector>
#include <string>
#include <fstream>

using std::string;
 
int num = 0;
int count = 4;
GLuint ibo, vbo, nbo, tbo, light, texture, programs[4], color[4], cam[4];
float camera_pos[4] = { 3, 4, 4, 1 };
double angle_hor = 0, angle_vert = 30, light_distance = 4;
float light_pos[4] = { 0, 0, 0, 1 };
float initial_figure_color[3] = { 0.9, 0.3, 0.7 };

string read_file(string path)
{
	string res = "";
	std::ifstream file(path);
	string line;
	getline(file, res, '\0');
	while (getline(file, line))
		res += "\n " + line;
	return res;
}

void init_start_params()
{
	glClearColor(0.5, 0.5, 0.5, 0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1, 0.1, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera_pos[0], camera_pos[1], camera_pos[2], 0, 0, 0, 0, 0, 1);
	int width, height;
	unsigned char* image = SOIL_load_image("texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void shader_init()
{
	string vertex_files[] = { "vertex0.shader", "vertex1.shader", "vertex2.shader", "vertex3.shader"};
	string fragment_files[] =  { "fragment0.shader", "fragment1.shader", "fragment2.shader", "fragment3.shader" };
	GLuint vertShader, fragmShader;

	for (size_t i = 0; i < count; ++i)
	{
		// получаем код
		string vertxs = read_file(vertex_files[i]);
		const char * vsSource = vertxs.c_str();
		string fragms = read_file(fragment_files[i]);
		const char * fsSource = fragms.c_str();

		// созание шейдеров
		vertShader = glCreateShader(GL_VERTEX_SHADER);
		fragmShader = glCreateShader(GL_FRAGMENT_SHADER);
		// передаем исходный код  
		glShaderSource(fragmShader, 1, &fsSource, NULL);
		glShaderSource(vertShader, 1, &vsSource, NULL);
		// компилируем шейдеры
		glCompileShader(vertShader);
		glCompileShader(fragmShader);

		// создаем программу
		programs[i] = glCreateProgram();
		//прикрепляем шейдеры 
		glAttachShader(programs[i], vertShader);
		glAttachShader(programs[i], fragmShader);
		// линкуем  
		glLinkProgram(programs[i]);
		 
		// получаем ID юниформ
		const char* cam_name = "camera_position";
		cam[i] = glGetUniformLocation(programs[i], cam_name); 
		const char* color_name = "color";
		color[i] = glGetUniformLocation(programs[i], color_name); 
	}
}

void buffers_init()
{
	int size = 2;
	GLfloat vertices[] =
	{
		0, 0, 0,
		0, 0, size ,
		0, size, 0,
		0, size, size,
		size, 0, 0,
		size, 0, size,
		size, size, 0,
		size, size, size
	};

	GLfloat normals[] = 
	{
		-1, -1, -1,
		-1, -1,  1,
		-1,  1, -1,
		-1,  1,  1,
		1, -1, -1,
		1, -1,  1,
		1,  1, -1,
		1,  1,  1
	};

	GLfloat textures[] = 
	{
		1.0, 0.0, //0
		0.0, 0.0, //1
		1.0, 1.0, //2
		0.0, 1.0, //3
		0.0, 1.0, //4
		1.0, 1.0, //5
		0.0, 0.0, //6
		1.0, 0.0  //7
	};

	GLubyte indices[] = 
	{
		0, 4, 6, 2,
		1, 5, 7, 3,
		4, 6, 7, 5,
		0, 2, 3, 1,
		0, 4, 5, 1,
		2, 6, 7, 3
	};

	// Связываем 
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &nbo);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals) * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textures) * sizeof(GLfloat), textures, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(GLubyte), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glUseProgram(programs[num]);

	glUniform3fv(cam[num], 1, camera_pos);
	glUniform3fv(color[num], 1, initial_figure_color);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glNormalPointer(GL_FLOAT, 0, NULL);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(programs[num], "our_texture"), 0);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glDrawElements(GL_QUADS, 24 * sizeof(GLubyte), GL_UNSIGNED_BYTE, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);

	// Отключаем шейдерную программу  
	glUseProgram(0);
	//checkOpe\nGLerror();
	glFlush();
	glutSwapBuffers();
}

void resizeWindow(int width, int height)
{
	glViewport(0, 0, width, height);
}

#pragma region keys
void special_keys(int key, int x, int y) {
	switch (key)
	{
		case GLUT_KEY_UP: angle_vert -= 5; break;
		case GLUT_KEY_DOWN: angle_vert += 5; break;
		case GLUT_KEY_RIGHT: angle_hor += 5; break;
		case GLUT_KEY_LEFT: angle_hor -= 5; break; 
	}
	double h = angle_hor / 180 * 3.1416;
	double v = angle_vert / 180 * 3.1416;
	light_pos[0] = light_distance * std::sin(v) * std::cos(h);
	light_pos[1] = light_distance * std::sin(v) * std::sin(h);
	light_pos[2] = light_distance * std::cos(v);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':angle_vert += 5; break;
		case 's': angle_vert -= 5; break;
		case 'a': angle_hor -= 5; break;
		case 'd': angle_hor += 5; break;
		case '-': light_distance++; break;
		case '=': light_distance--; break;
		case '1': num = 0; break;
		case '2': num = 1; break;
		case '3': num = 2; break;
		case '4': num = 3; break;
	}
	double h = angle_hor / 180 * 3.1416;
	double v = angle_vert / 180 * 3.1416;
	light_pos[0] = light_distance * std::sin(v) * std::cos(h);
	light_pos[1] = light_distance * std::sin(v) * std::sin(h);
	light_pos[2] = light_distance * std::cos(v);
	glutPostRedisplay();
}
#pragma endregion

#pragma region free_resources
void shader_free()
{
	glUseProgram(0);
	for (int i = 0; i < count; ++i)
		glDeleteProgram(programs[i]);
}

void buffers_free()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &nbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ibo);
}
#pragma endregion

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Task14");

	// инициализация  GLEW
	GLenum glew_status = glewInit();  
	
	// Инициализация  компонентов
	init_start_params();
	buffers_init();
	shader_init();
	glutReshapeFunc(resizeWindow);
	glutDisplayFunc(render);
	glutSpecialFunc(special_keys);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	
	shader_free();
	buffers_free();
}
