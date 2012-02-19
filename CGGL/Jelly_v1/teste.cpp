//including windows, gl and glut for graphics
//stdio for buffering
#include <windows.h>
#include <gl\GL.h>
//#include <glut.h>
#include <gl\glut.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

//hold display list
GLuint displist; 

//two variables for window size
int width = 500;
int height = 500;


//orthogonal projection
void orthogonalStart (void) {
    //switch to projection matrix
    glMatrixMode(GL_PROJECTION);

    //start projections
    glPushMatrix();

    //clear previous instructions
    glLoadIdentity();

    //change window size to variables defined
    gluOrtho2D(0, width, 0, height);

    //switch back to model matrix
    glMatrixMode(GL_MODELVIEW);
}

void orthogonalEnd (void) {
    //Switch back to projection mode
    glMatrixMode(GL_PROJECTION);

    //Finish the calls above
    glPopMatrix();

    //Switch back to our model matrix
    glMatrixMode(GL_MODELVIEW);
}

void display (void) {

    //clearing draw buffer painting bkg red
    glClearColor (1.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

     //calling function
         orthogonalStart();

     //declare displist as display list 
     displist = glGenLists(1);

     //compile the new list
     glNewList(displist,GL_COMPILE); 

     //start projections
     glPushMatrix();


     for(int i=0; i<=4; i++){

        //draw a square
        //QUADSTRIP Improvement?
        glBegin(GL_QUADS);
        glColor3f(0,0,1);
        glVertex2f(100, 150);
        glColor3f(0,1,1);
        glVertex2f(100, 100);
        glColor3f(1,0,1);
        glVertex2f(400, 100);
        glColor3f(1,1,1);
        glVertex2f(400, 150);



        //end drawing
        glEnd();

        //end projections
        glPopMatrix();

        //translate start point
        glTranslatef(0, 75, 0);

       glFlush();

    }

   //end the list
       glEndList(); 


//call displist
glCallList(displist);

//calling function
orthogonalEnd();

//move contents of back buffer to front buffer
glutSwapBuffers();
glLoadIdentity ();
glFlush();
}

int main(int argc, char * argv []){

glutInit (&argc, argv);
glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
glutInitWindowSize (500, 500);
glutInitWindowPosition (0, 0);
glutCreateWindow ("A Basic Window");
glutDisplayFunc (display);
glutMainLoop ();
return 0;
}