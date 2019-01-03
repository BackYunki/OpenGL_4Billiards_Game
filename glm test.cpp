#include "libs.h"
vec4 origin = vec4(0, 0, 0, 1);
vec4 pos = vec4(200, 0, 0, 1);
vec4 r;

vec4 v = vec4(0, 60, 0, 0);
vec4 a;

GLdouble dt = 0.f;

int main()
{
	r = (pos - origin);
	printf("x: %.2lf y: %.2lf z: %.2lf w: %.2lf\n", r.x, r.y, r.z, r.w);
	return 0;
}