//Гуро, без текстур
varying float lightness;
uniform vec3 camera_position;

void main(void) 
{
	//перевод координат вершины в мировую систему координат
	vec3 p = vec3(gl_ModelViewMatrix * gl_Vertex);
	//Направление падающего света, от вершины к источнику освещения (light direction)
	vec3 l = normalize(vec3(gl_LightSource[0].position) - p);
	//Нормаль к поверхности объекта в вершине (normal) в мировой системе координат
	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	//Направление взгляда, от вершины к наблюдателю (view direction) в мировой системе координат
	vec3 v = normalize(camera_position - p);
	vec3 i = l * (-1.0);
	//Расстояние от точечного источника освещения до вершины (distance)
	vec3 r = normalize(i - n * 2 * dot(n, i));
	//итоговый свет. считаем и передаем в фрагментный шейдер
	lightness = 0.2 
	//добавление рассеянного света
	+ max(dot(n, l), 0.0) 
	//добавление отраженного света
	+ pow(max(dot(v, r), 0.0), 5);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

