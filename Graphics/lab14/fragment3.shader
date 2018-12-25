//Фонг, с текстурами
#version 330 core
varying vec3 l;
varying vec3 n;
varying vec3 v;
varying vec3 r;
varying vec2 tex;
uniform sampler2D our_texture;

void main(void) 
{
	//высчитываем свет
	float lightness = 0.2  
	//добавление рассеянного света
	+ max(dot(n, l), 0.0) 
	//добавление отраженного света
	+ pow(max(dot(v, r), 0.0), 5);
	// вычислим итоговый цвет пикселя на экране с учётом текстуры
	vec3 tex_clr = vec3(texture(our_texture, tex) * lightness);
	//возвращяем в основную программу цвет
	gl_FragColor = vec4(tex_clr, 1.0);
}