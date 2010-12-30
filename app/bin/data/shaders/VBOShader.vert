
attribute float particleSize;

void main() {
	
	vec4 eyeCoord = gl_ModelViewMatrix * gl_Vertex;
	gl_Position = gl_ProjectionMatrix * eyeCoord;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_PointSize = particleSize;
	gl_FrontColor = gl_Color;
	
	
}





