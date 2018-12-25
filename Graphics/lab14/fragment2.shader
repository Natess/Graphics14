//Гуро, с текстурами
#version 330 core
varying float lightness;
varying vec2 tex;
uniform sampler2D our_texture;

void main(void) 
{
	// вычислим итоговый цвет пикселя на экране с учётом текстуры
	vec3 tex_clr = vec3(texture(our_texture, tex) * lightness);
	//возвращяем в основную программу цвет
	gl_FragColor = vec4(tex_clr, 1.0);
}