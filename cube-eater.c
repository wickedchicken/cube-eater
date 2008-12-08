/*
 * cubeeater
 * by Mike Stipicevic
 *
 * controls: wasd, yYxXzZ
 * attempt to pick up as many cubes as you can
 *
 * this code is based on 'cubeview.c' from Chapter 5
 * and includes code from websites where noted
 *
 * http://www.cs.unm.edu/~angel/BOOK/INTERACTIVE_COMPUTER_GRAPHICS/FIRST_EDITION/programs/cubeview.c
 *
 * to build and run this application, view the README
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "simulation.h"
#include "spaceship.h"
#include "vecops.h"
#include "skybox.h"
#include "cube.h"
#include <math.h>

// the step size of the simulation, right now 32 times a second
const unsigned long granularity = 1000/32;

void display(void);
void colorcube_r();
GLuint build_colorcube();
GLuint build_colorcube_r();

// the rotation of the ship
static GLfloat theta[] = {-90.0,0.0,0.0};

// the rotation of the camera 
static GLfloat camtheta[] = {0.0,0.0,0.0};
static GLint axis = 2;

static GLfloat viewer[]= {0.0, 4.0, 8.0}; /* initial viewer location */
static float viewlen;

// display lists for ship, cube, red cube, and skybox
static GLuint ship;
GLuint cube;
GLuint cube_r;
static GLuint sky;

// skybox textures
GLuint skybox[6];

// capture state of keypresses
enum motiontypes {NO_MOTION,LEFT_MOTION,RIGHT_MOTION} motion = NO_MOTION;
enum formotiontypes {NOF_MOTION,FORWARD_MOTION,BACK_MOTION} frmotion = NOF_MOTION;
char leftdown;
char rightdown;
char updown;
char downdown;

// integral term of tilt, camera, yaw and speed for PID
float integ_tilt = 0;
float lastval_tilt = 0;
float integ_cam = 0;
float lastval_cam = 0;
float integ_yaw = 0;
float lastval_yaw = 0;
float integ_accel = 0;
float lastval_accel = 0;

// ship position
double shipx;
double shipy;

// ship state
float shiphdg=0;
float shipyaw=0;
float shipaccel=0;
float shipspeed=0;

// main simulation loop
static void timerCallback (int value) {

	// setup PID coefficients
	float dest[3] = {-90,0,0};
	float dest_cam[3] = {0,0,0};
	float dest_yaw[3] = {0,0,0};
	float dest_accel[3] = {0,0,0};
	float yawvec[3] = {shipyaw,0,0};
	float accelvec[3] = {shipaccel,0,0};
	const float p_tilt = 0.3;
	const float i_tilt = 0.3;
	const float d_tilt = 0.1;
	const float p_cam = 0.05;
	const float i_cam = 0.3;
	const float d_cam = 1.1;
	const float p_yaw = 0.3;
	const float i_yaw = 0.3;
	const float d_yaw = 0.1;
	const float p_accel = 0.005;
	const float i_accel = 0.3;
	const float d_accel = 1.1;


	/* Do timer processing */
	/* call back again after elapsedUSecs have passed */
	glutTimerFunc (granularity, timerCallback, value);

	if(motion == LEFT_MOTION){
		dest[1] = -30;
		dest_cam[1] = -30;
		dest_yaw[0] = -0.7;
	}
	if(motion == RIGHT_MOTION){
		dest[1] = 30;
		dest_cam[1] = 30;
		dest_yaw[0] = 0.7;
	}

	if(frmotion == FORWARD_MOTION){
		dest_accel[0] = 6;
	}
	if(frmotion == BACK_MOTION){
		dest_accel[0] = -6;
	}

	// setup camera
	pid(camtheta,dest_cam,p_cam,i_cam,d_cam,&lastval_cam,&integ_cam);
	// setup ship tilt
	pid(theta,dest,p_tilt,i_tilt,d_tilt,&lastval_tilt,&integ_tilt);
	// setup ship yaw
	pid(yawvec,dest_yaw,p_yaw,i_yaw,d_yaw,&lastval_yaw,&integ_yaw);
	shipyaw = yawvec[0];

	// setup ship speed
	pid(accelvec,dest_accel,p_accel,i_accel,d_accel,&lastval_accel,&integ_accel);
	shipaccel = accelvec[0];

	// display everything
	display();
}

// spent cube buffer
long seen[50*2];
// length of spent cube buffer
const int seenbuffer = 50;
// buffer is circular -- current count
int seenrollcount = 0;

// cubes picked up by player
unsigned long pointz = 0;


// generate a field of cubes based on current location
void displaysomeCUBES(const long x, const long y){
	GLdouble const * grid = fillgrid(x,y);
	int i=0;
	int disp = 1;

	long count = grid[0]-1;

	for(;count>=0;count--){
		if(grid[3+count*3] == 1){
			disp  =1;
			// get a better data structure
			for(i=0;i<seenbuffer;i++){
				if((seen[i*2] == grid[1+count*3]) && (seen[1+(i*2)] == grid[2+count*3])){
					disp = 0;
					break;
				}
			}
			if((grid[1+count*3] < (x+4))&&
					(grid[1+count*3] > (x-4)) &&
					(grid[2+count*3] < (y+10)) &&
					(grid[2+count*3] > (y+6))){
				// cube wasn't in the buffer, it must be eaten
				seen[seenrollcount*2] = grid[1+count*3];
				seen[1+(seenrollcount*2)] = grid[2+count*3];
				seenrollcount++;
				seenrollcount %= seenbuffer;
				pointz++;
				// make it red
				disp = 0;
			}
			glPushMatrix();
			// translate to cube location
			glTranslated(grid[1+count*3],-1,-grid[2+count*3]);
			if(disp){
				colorcube();
			}
			else{
				colorcube_r();
			}
			glPopMatrix();
		}
	}

	free(grid);
}

// display ship, cubes, and skybox
void display(void)
{
	float xangle;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// display sky
	rendersky(sky);

	// ignore miniscule changes
	if(fabs(shipyaw) > 0.01){
		shiphdg += shipyaw;
	}

	// make ship heading b/t 0 and 360
	if(shiphdg >= 360){
		shiphdg -= 360;
	}
	else{
		if(shiphdg <= 0){
			shiphdg += 360;
		}
	}

	// calculate new ship x and y based on heading and speed
	shipx += sin((shiphdg/180.0) * M_PI)*shipaccel;
	shipy += cos((shiphdg/180.0) * M_PI)*shipaccel;


	glLoadIdentity();
	gluLookAt(viewer[0],viewer[1],viewer[2], 0.0, 4.0, 0.0, 0.0, 1.0, 0.0);

	// calculate camera angle
	xangle = (camtheta[1]);
	if(xangle >= 360){
		xangle -= 360;
	}
	else{
		if(xangle <= 0){
			xangle += 360;
		}
	}

	// display cubes
	glPushMatrix();
	glTranslatef(-viewer[0],-viewer[1]+4,-viewer[2]);
	glRotatef(xangle,0.0,1.0,0.0);
	glRotatef(shiphdg,0.0,1.0,0.0);
	glTranslatef(viewer[0],viewer[1]-4,viewer[2]);
	glTranslated(-shipx,0,shipy);
	displaysomeCUBES((long)shipx,(long)shipy);
	glPopMatrix();

	// display ship
	glTranslatef(-viewer[0],-viewer[1]+4,-viewer[2]);
	glRotatef(camtheta[0], 1.0, 0.0, 0.0);
	glRotatef(xangle, 0.0, 1.0, 0.0);
	glRotatef(camtheta[2], 0.0, 0.0, 1.0);
	glTranslatef(viewer[0],viewer[1]-4,viewer[2]);
	glPushMatrix();
	glTranslatef(-viewer[0],-viewer[1]+4,-viewer[2]);
	spaceship(0,0,10,theta,ship);
	glPopMatrix(); 

	glFlush();
	glutSwapBuffers();
}

// keep track of when keys are let go
void keysup(unsigned char key, int x, int y){
	if(key == 'a'){
		leftdown = 0;
		if(rightdown){
			motion = RIGHT_MOTION;
		}
		else{
			motion = NO_MOTION;
		}
	}
	if(key == 'd'){
		rightdown = 0;
		if(leftdown){
			motion = LEFT_MOTION;
		}
		else{
			motion = NO_MOTION;
		}
	}
	if(key == 'w'){
		updown = 0;
		if(downdown){
			motion = BACK_MOTION;
		}
		else{
			frmotion = NOF_MOTION;
		}
	}
	if(key == 's'){
		downdown = 0;
		if(updown){
			frmotion = FORWARD_MOTION;
		}
		else{
			frmotion = NOF_MOTION;
		}
	}
}

// keep track of when keys are pressed
void keys(unsigned char key, int x, int y)
{

   if(key == 'x') viewer[0]-= 1.0;
   if(key == 'X') viewer[0]+= 1.0;
   if(key == 'y') viewer[1]-= 1.0;
   if(key == 'Y') viewer[1]+= 1.0;
   if(key == 'z') viewer[2]-= 1.0;
   if(key == 'Z') viewer[2]+= 1.0;
   if(key == 'a') {leftdown = 1;motion = LEFT_MOTION;}
   if(key == 'd') {rightdown = 1;motion = RIGHT_MOTION;}
   if(key == 'w') {updown = 1;frmotion = FORWARD_MOTION;}
   if(key == 's') {downdown = 1;frmotion =BACK_MOTION;}

   display();
}

// screen width, height
int screenw,screenh;

// set up view frustum
void setupproj(){
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(45.0, (double)screenw/(double)screenh, 2.0, 600.0); 
	glMatrixMode(GL_MODELVIEW);
}

// recalculate on window resize
void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	screenw = w;
	screenh = h;

	/* Use a perspective view */
	setupproj();
}

// initalize skybox and display lists
void initialize(){
	leftdown = 0;
	rightdown = 0;
	viewlen = veclen(viewer);
	skybox[0] = LoadTextureRAW("skybox/Front_Muddy.jpg_rgb",0);
	skybox[2] = LoadTextureRAW("skybox/Back_Muddy.jpg_rgb",0);
	skybox[5] = LoadTextureRAW("skybox/Down_Muddy.jpg_rgb",0);
	skybox[1] = LoadTextureRAW("skybox/Left_Muddy.jpg_rgb",0);
	skybox[3] = LoadTextureRAW("skybox/Right_Muddy.jpg_rgb",0);
	skybox[4] = LoadTextureRAW("skybox/Up_Muddy.jpg_rgb",0);
	ship = buildship();
	cube = build_colorcube();
	cube_r = build_colorcube_r();
	sky = buildrendersky(skybox);
}


// main program
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("cube eater");
	glutReshapeFunc(myReshape);
	glLoadIdentity();
	initialize();


	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	glutKeyboardUpFunc(keysup);
	glEnable(GL_DEPTH_TEST);
	glutTimerFunc (granularity, timerCallback, 0);
	glutMainLoop();
	return 0;
}
