//Фонг, без текстур
varying vec3 l;
varying vec3 n;
varying vec3 v;
varying vec3 r;
uniform vec3 color;

void main(void) 
{
	//высчитываем результирующий цвет
	float lightness = 0.2 
	//добавление рассеянного света
	+ max(dot(n, l), 0.0) 
	//добавление отраженного света
	+ pow(max(dot(v, r), 0.0), 5);
	//возвращяем в основную программу цвет
	gl_FragColor = vec4(color * lightness, 1.0);
}