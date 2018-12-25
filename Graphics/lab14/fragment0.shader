//Гуро, без текстур
varying float lightness;
uniform vec3 color;

void main(void) 
{
	//возвращяем в основную программу цвет
	gl_FragColor = vec4(color * lightness, 1.0);
}