/*
 * @ Autor ANDRE
 *  
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#define v .577350269

GLfloat lmodel_ambient[4]={0.2,0.2,0.2,1.0};
/* posicao da fonte */
GLfloat light_position[4];
/* intensidade ambiente (Ia) */
GLfloat light_ambient[4];
/* intensidade lambertiana/difusa/incoerente (Id) */
GLfloat light_diffuse[4];
/* intensidade especular (Is) */
GLfloat light_specular[4];
/* direcao da fonte spot (d) */
GLfloat spot_direction[3];
/* expoente de decaimento da intensidade (c) */
GLfloat spot_exponent;
/* angulo de abrangencia da fonte spot (gamma) */
GLfloat spot_cutoff;
/* coeficiente do termo constante do fator de atenuacao (c_1) */
GLfloat fat_constant;
/* coeficiente do termo linear do fator de atenuacao (c_2) */
GLfloat fat_linear;
/* coeficiente do termo quadratico do fator de atenuacao (c_3) */
GLfloat fat_quadratic;

GLfloat c = 1.73205080756887729, length;

/* Material do Objeto */
GLfloat mat_ambient[4];   /* Ka*/
GLfloat mat_diffuse[4];   /* Kd */
GLfloat mat_specular[4];  /* Ks */
GLfloat mat_shininess;                            /* n */
GLfloat mat_emission[4];

GLenum especular=GL_TRUE, difuso=GL_TRUE, ambiente=GL_TRUE;

GLenum light0=GL_TRUE, light1=GL_TRUE;

GLfloat dX = 0.0, dY = 0.0, dZ = 0.0;
GLfloat rotX = 0.0, rotY = 0.0, rotZ = 0.0;


static GLfloat vdata[8][3] = {
   {-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0}, {1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0},
   {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0}
   };

static GLfloat ndata[8][3] = {
   {-v, -v, -v}, {v, -v, -v}, {v,v,-v}, {-v, v, -v},
   {-v, -v, v}, {v, -v, v}, {v,v,v}, {-v, v, v}
   };

static GLuint vindices[6][4] = {
   {0,3,2,1}, {2,3,7,6}, {0,4,7,3},
   {1,2,6,5}, {4,5,6,7}, {0,1,5,4}
   };

static GLfloat m[4][4] = {
   {1.0,0.0,0.0,0.0}, {0.0,1.0,0.0,0.0}, {0.0,0.0,1.0,0.0}, {0.0,0.0,0.0,1.0}
   };

GLfloat spin_x=0.0, spin_y=0.0, spin_z=0.0;

GLfloat ant=0.0, bra=0.0, cab=0.0;

GLuint braco, busto, cabeca, perna;

GLdouble w;

/* Desenhar um cubo */

static void cube()
{
  int i;

   /* desenhar o cubo */
   for (i=0; i<6; i++) {
     glBegin(GL_POLYGON);
     glColor3f (1.0, 0.0, 0.0);
     glNormal3fv(&ndata[vindices[i][0]][0]);
     glVertex3fv(&vdata[vindices[i][0]][0]);
     glColor3f (0.0, 1.0, 0.0);
     glNormal3fv(&ndata[vindices[i][1]][0]);
     glVertex3fv(&vdata[vindices[i][1]][0]);
     glColor3f (0.0, 0.0, 1.0);
     glNormal3fv(&ndata[vindices[i][2]][0]);
     glVertex3fv(&vdata[vindices[i][2]][0]);
     glColor3f (0.0, 1.0, 1.0);
     glNormal3fv(&ndata[vindices[i][3]][0]);
     glVertex3fv(&vdata[vindices[i][3]][0]);
     glEnd();
   }
}

static void boneco()
{
   /* criar braco */
  /* gerar um indice de uma lista */
   braco = glGenLists(1);
   /* inicializar o conteudo da lista "braco". */

   glNewList(braco, GL_COMPILE);
   glScalef(0.09, 0.28, 0.09);
   cube();
   /* finalizar a lista inicializada */
   glEndList();

   /* criar busto */
   busto = glGenLists(1);
   glNewList(busto, GL_COMPILE);
   glScalef(0.35, 0.525, 0.2);
   cube();
   glEndList();

   /* criar cabeca */
   cabeca = glGenLists(1);
   glNewList(cabeca, GL_COMPILE);
   glScalef(0.2, 0.2, 0.2);
   cube();
   glEndList();

   /* criar perna */
   perna = glGenLists(1);
   glNewList(perna, GL_COMPILE);
   glScalef(0.09, 0.57, 0.09);
   cube();
   glEndList();

}

void init(void)
{
   /* definir dados do boneco */
   boneco();
   /* definir a cor em preto (0,0,0) como cor de "CLEAR" */
   glClearColor (0.0, 0.0, 0.0, 0.0);
   /* setar o tipo de tonalizacao. Ha duas alternativas
      GL_FLAT (constante) e GL_SMOOTH (interpolacao linear
      entre os vertices) */
   glShadeModel (GL_SMOOTH);
   
   glEnable(GL_LIGHTING);
   
   /* setar o tamanho do ponto em pixels */
   glPointSize(5.0);
   /* setar a largura da linha em pixels */
   glLineWidth(3.0);
   /* setar o modo de renderizacao das faces. Ha tres alternativas:
      GL_POINT (somente vertices), GL_LINE (somente linhas) e GL_FILL
      (faces preenchidas) */
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   /* setar a convencao de orientacao. Ha duas alternativas: GL_CCW (anti-horario)      e GL_CW (horario) */
   glFrontFace (GL_CCW);
   /* Habilitar face culling */
   glEnable (GL_CULL_FACE);
   /* definir as faces a serem discardadas. Ha tres alternativas: GL_FRONT,
      GL_BACK e GL_FRONT_AND_BACK */
   glCullFace (GL_BACK);
   
      /* setar uma luz de ambiente */
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
   
   
}

void Light0 (void)
{
    /****************** Fonte 0  (Vermelha) *****************/
    light_position[0] = -w-1.0; light_position[1] = -w-1.0; light_position[2] = -w-1.0; light_position[3] = 1.0;
    light_ambient[0] = 1.0; light_ambient[1] = 0.2; light_ambient[2] = 0.2; light_ambient[3] = 1.0;
    light_diffuse[0] = 1.0; light_diffuse[1] = 0.2; light_diffuse[2] = 0.2; light_diffuse[3] = 1.0;
    light_specular[0] = 1.0; light_specular[1] = 1.0; light_specular[2] = 1.0; light_specular[3] = 1.0;
    spot_direction[0] = 1./sqrt(3.); spot_direction[1] = 1./sqrt(3.); spot_direction[2] = 1./sqrt(3.);

    spot_exponent = 5.0; /* default: 0.0 */
    spot_cutoff = 45.0;  /* angulo entre 0 a 90 */
    fat_constant = 1.0;  /* default: 1.0 */
    fat_linear = 0.0;    /* default: 0.0 */
    fat_quadratic = 0.0; /* deafult: 0.0 */

    /* Definir a posicao da luz */
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);

    /* Definir as intensidades ambiente, difusa e especular */
    glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);

    /* Caso for fonte spot, definir a direcao, o expoente de decaimento e angulo */
    glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightf (GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
    glLightf (GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);

    /* Definir o fator de atenuacao da intensidade da fonte */
    glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, fat_constant);
    glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, fat_linear);
    glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, fat_quadratic);

    glDisable(GL_LIGHTING); /* desabilitar o modelo de iluminacao para
                               colorir o ponto com a "cor" especificada */
    /* desenhar a posicao da fonte */
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(4.0);
    glBegin(GL_POINTS);
     glVertex3fv(light_position);
    glEnd();
    glPointSize(1.0);

    /* desenhar a direcao do feixe */
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
     glVertex3fv(light_position);
     glVertex3f(light_position[0]+spot_direction[0],light_position[1]+spot_direction[1],
                 light_position[2]+spot_direction[2]);
    glEnd();

    glEnable(GL_LIGHTING); /* reabilitar o modelo de iluminacao */
}

void Light1 (void)
{
    /****************** Fonte 1 (Azul) ******************/
    light_position[0] = length+1.0; light_position[1] = length+1.0; light_position[2] = length+1.0; light_position[3] = 1.0;
    light_ambient[0] = 0.2; light_ambient[1] = 0.2; light_ambient[2] = 1.0; light_ambient[3] = 1.0;
    light_diffuse[0] = 0.2; light_diffuse[1] = 0.2; light_diffuse[2] = 1.0; light_diffuse[3] = 1.0;
    light_specular[0] = 1.0; light_specular[1] = 1.0; light_specular[2] = 1.0; light_specular[3] = 1.0;
    spot_direction[0] = -1.0/sqrt(3.); spot_direction[1] = -1.0/sqrt(3.); spot_direction[2] = -1.0/sqrt(3.);

    spot_exponent = 0.0;
    spot_cutoff = 60.0;
    fat_constant = 1.0;
    fat_linear = 0.0;
    fat_quadratic = 0.0;

    /* Definir a posicao da luz */
    glLightfv (GL_LIGHT1, GL_POSITION, light_position);

    /* Definir as intensidades ambiente, difusa e especular */
    glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular);

    /* Caso for fonte spot, definir a direcao, o expoente de decaimento e angulo */

    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, spot_exponent);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, spot_cutoff);

    /* Definir o fator de atenuacao da intensidade da fonte */
    glLightf (GL_LIGHT1, GL_CONSTANT_ATTENUATION, fat_constant);
    glLightf (GL_LIGHT1, GL_LINEAR_ATTENUATION, fat_linear);
    glLightf (GL_LIGHT1, GL_QUADRATIC_ATTENUATION, fat_quadratic);

    glDisable(GL_LIGHTING); /* desabilitar o modelo de iluminacao para
                               colorir o ponto com a "cor" especificada */
    /* Desenhar a posicao da fonte */
    glColor3f(0.0, 0.0, 1.0);
    glPointSize(4.0);
    glBegin(GL_POINTS);
     glVertex3fv(light_position);
    glEnd();
    glPointSize(1.0);
    /* Desenhar a direcao do feixe */
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
     glVertex3fv(light_position);
     glVertex3f(light_position[0]+spot_direction[0], light_position[1]+spot_direction[1],
                 light_position[2]+spot_direction[2]);
    glEnd();
    glEnable(GL_LIGHTING); /* reabilitar o modelo de iluminacao */
}

void MaterialCube()
{
    mat_emission[0] = 0.0; mat_emission[1] = 0.0; mat_emission[2] = 0.0; mat_emission[3] = 1.0;

    if (especular) {
          mat_specular[0] = 1.0; mat_specular[1] = 1.0; mat_specular[2] = 1.0; mat_specular[3] = 1.0;
          mat_shininess = 50.0;
    } else {
          mat_specular[0] = mat_specular[1] = mat_specular[2] = mat_specular[3] = 0.0;
    }

    if (ambiente) {
      mat_ambient[0] = 0.2; mat_ambient[1] = 0.2; mat_ambient[2] = 0.2; mat_ambient[3] = 1.0;
    } else {
      mat_ambient[0] = mat_ambient[1] = mat_ambient[2] = mat_ambient[3] = 0.0;
    }

    if (difuso) {
      mat_diffuse[0] = 0.3; mat_diffuse[1] = 0.4; mat_diffuse[2] = 0.8; mat_diffuse[3] = 1.0;
    } else {
      mat_diffuse[0] = mat_diffuse[1] = mat_diffuse[2] = mat_diffuse[3] = 0.0;
    }

    glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf (GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv (GL_FRONT, GL_EMISSION, mat_emission);
}


void display(void)
{
   int i;

   /* "CLEAR" o display */
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   /* setar a cor de desenho em branco (1,1,1) */
   glColor3f (1.0, 1.0, 1.0);
   /* carregar a matriz de identidade na pilha de matriz de transformacao */
   glLoadIdentity();
    /* Habilitar as fontes */
    if (light0) glEnable(GL_LIGHT0);
    else glDisable(GL_LIGHT0);
    if (light1) glEnable(GL_LIGHT1);
    else glDisable (GL_LIGHT1);
   /* concatenar a matriz de translacao com a do topo da pilha */
   glTranslatef(0.0,0.0,-2.0);

   /* concatenar a matriz de rotacao com a do topo da pilha */
   glRotatef(spin_x,1.0,0.0,0.0);

   /* concatenar a matriz de rotacao com a do topo da pilha */
   glRotatef(spin_y,0.0,1.0,0.0);

   /* concatenar a matriz de rotacao com a do topo da pilha */
   glRotatef(spin_z,0.0,0.0,1.0);

   /* definir matriz de cisalhamento */
   m[0][0] = 1.0; m[1][0] = 0.0;  m[2][0] = 0.0;  m[3][0] = 0.0;
   m[0][1] = 0.0; m[1][1] = 1.0;  m[2][1] = 0.0;  m[3][1] = 0.0;
   m[0][2] = 0.0; m[1][2] = 0.0;  m[2][2] = 1.0;  m[3][2] = 0.0;
   m[0][3] = 0.0; m[1][3] = 0.0;  m[2][3] = 0.0;  m[3][3] = 1.0;
   /* concatenar a matriz com a do topo da pilha */
   glMultMatrixf(&m[0][0]);

   /* concatenar a matriz de mudanca de escala com a do topo da pilha */
   glScalef(1.,1.,1.);

   /* desenhar a figura */
 
   /* desenhar o busto */
   glPushMatrix();
   glCallList(busto);
   MaterialCube();
   glPopMatrix();

   /* desenhar as pernas */
   glPushMatrix();
   glTranslatef(0.2, -1.12, 0.0);
   glCallList(perna);
   MaterialCube();
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-0.2, -1.12, 0.0);
   glCallList(perna);
   MaterialCube();
   glPopMatrix();

   /* desenhar a cabeca */
   glPushMatrix();
   glTranslatef(0.0, 0.73, 0.0);
   // rotacionar a cabeca no sentido anti-horario em torno de y */
   glRotatef(cab, 0.0, 1.0, 0.0);
   glCallList(cabeca);
   MaterialCube();
   glPopMatrix();

   /* desenhar os bracos */
   glPushMatrix();
   glTranslatef(-0.45, 0.26, 0.0);
   glTranslatef(.0, .28, .0);
   glRotatef(bra, 1.0, 0.0, 0.0);
   glTranslatef(.0, -.28, .0);
   glPushMatrix();
   glCallList(braco);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, -0.59, 0.0);
   glTranslatef(.0, .28, .0);
   glRotatef(ant, 1.0, 0.0, 0.0);
   glTranslatef(.0, -0.28, .0);
   glCallList(braco);
   glPopMatrix();
   glPopMatrix();



   glPushMatrix();
   glTranslatef(0.45, 0.26, 0.0);
   /* rotationar o braco no sentido anti-horario em trono do x */
   glTranslatef(.0, .28, .0);
   glRotatef(bra, 1.0, 0.0, 0.0);
   glTranslatef(.0, -.28, .0);
   glPushMatrix();
   glCallList(braco);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0., -0.59, 0.0);
   /* rotationar o anti-braco no sentido anti-horario em trono do x */
   glTranslatef(.0, .28, .0);
   glRotatef(ant, 1.0, 0.0, 0.0);
   glTranslatef(.0, -0.28, .0);
   glCallList(braco);
   glPopMatrix();
   glPopMatrix();

   /* forcar a execucao dos comandos enviados a OpenGL */
   glFlush ();
}

void reshape (int w, int h)
{
   /* definir o tamanha de janela no display, em pixels */
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   /* chavear para a pilha de transformacao de projecao */
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glOrtho (-2.0, 2.0, -2.0, 2.0, 0.0, 5.0);
   /* chavear para a pilha de transformacao de modelo */
   glMatrixMode (GL_MODELVIEW);
}

/* ARGSUSED1 */
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 'a':
        /* increment -10.0 no angulo de rotacao em torno do eixo x */
        ant -= 10.0;
        if (ant < -160.0) ant = 0.0;
        break;
      case 'b':
        /* increment -10.0 no angulo de rotacao em torno do eixo x */
        bra -= 10.0;
        if (bra < -160.0) bra = 0.0;
        break;
      case 'c':
        /* increment -10.0 no angulo de rotacao em torno do eixo x */
        cab += 10.0;
        break;
      case 'd':
        if (difuso) difuso = GL_FALSE; else difuso = GL_TRUE;
        break;
      case 'e':
        if (especular) especular = GL_FALSE; else especular = GL_TRUE;
        break;
      case 'x':
        /* increment +10.0 no angulo de rotacao em torno do eixo x */
	    spin_x += 10.0;
        break;
      case 'y':
        /* incrementa +10.0 no angulo de rotacao em torno do eixo y */
	spin_y += 10.0;
        break;
      case 'z':
        /* increment +10.0 no angulo de rotacao em torno do eixo z */
	spin_z += 10.0;
        break;
      case 'r':
        /* reset */
	spin_x = spin_y = spin_z = 0.0;
        break;
      case 27:
         exit(0);
         return;
   }
   /* formar um evento para ativar o redesenho */
   glutPostRedisplay();
}

int main(int argc, char** argv)
{
   /* estabelecimento de comunicacao com o sistema de janela */
   glutInit(&argc, argv);
   /* setar o " visual" do display */
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   /* definir a janela */
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   /* inicializar openGL */
   init ();
   /* registrar os tratadores de eventos */
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   /* entre no modo de espera */
   glutMainLoop();
   return 0;
}
