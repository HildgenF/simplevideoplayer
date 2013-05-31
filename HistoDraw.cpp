#include "HistoDraw.h"

HistoDraw::HistoDraw(QWidget *parent) : QGLWidget(parent) {

    setMouseTracking(true);

	w = 0;
	h = 0;
	display = false;

	first_frame = true;
	_extClose = false;
	histo_height = 100; //Altezza istogramma in pixel

	R.resize(256, 0);
	G.resize(256, 0);
	B.resize(256, 0);
}

HistoDraw::~HistoDraw(){

}

void HistoDraw::initializeGL() {
}

void HistoDraw::resizeGL(int w, int h) {
    
	glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void HistoDraw::paintGL() {

	/** non faccio niente fino a quando non avr� un riferimento con valore */
	if(R.size() == NULL){
		return;
	};

    //dentro a questo if ci adr� una volta sola!
	if(first_frame){
		this->initializeGL();
		first_frame = false;
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear The Screen And The Depth Buffer
    glLoadIdentity();                       // Reset The View
	glClearColor(0.0,0.0,0.0,0.0); 
    glMatrixMode(GL_2D); 
    gluOrtho2D(0.0,300.0,0.0,300.0);
    glPointSize(3.0);  

	glColor3f(0.0,0.0,1.0); 
    glBegin(GL_LINES);
	for(unsigned i=0; i<B.size(); ++i){
		glVertex2d(i, 0);
		glVertex2d(i, B[i]);
	}
    glEnd();

	glColor3f(0.0,1.0,0.0); 
    glBegin(GL_LINES);
	for(unsigned i=0; i<G.size(); ++i){
		glVertex2d(i, 101);
		glVertex2d(i, 101+G[i]);
	}
    glEnd();

	glColor3f(1.0,0.0,0.0); 
    glBegin(GL_LINES);
	for(unsigned i=0; i<R.size(); ++i){
		glVertex2d(i, 202);
		glVertex2d(i, 202+R[i]);
	}
    glEnd();
	

}

void HistoDraw::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_Escape:
		emit Xpressed();
        break;
    default:
        event->ignore();
        break;
    }
}

void HistoDraw::setSize(int w, int h){
	this->w = w;
	this->h = h;
}

void HistoDraw::setValues(std::vector<int>& R1, std::vector<int>& G1, std::vector<int>& B1){
	/*
		Quando prendo i valori devo riscalarli in modo da dargli un senso a livello dell'istogramma.
		Quindi li porto nel range in modo che il valore massimo della distribuzione venga portato pari al valore massimo dell'istogramma.
	*/
	int maxR = *std::max_element(R1.begin(), R1.end()); /* Valore massimo rosso*/
	int maxG = *std::max_element(G1.begin(), G1.end()); /* Valore massimo verde*/
	int maxB = *std::max_element(B1.begin(), B1.end()); /* Valore massimo blu*/

	for(unsigned i=0; i<R1.size(); ++i){
		this->R[i] = (float) (R1[i]*histo_height)/maxR;
		this->G[i] = (float) (G1[i]*histo_height)/maxG;
		this->B[i] = (float) (B1[i]*histo_height)/maxB;
	}
}

void HistoDraw::startdisplay(void){
	display = true;	
};


////////////////////////////////////////////////////////////////////////////////////

/**
SLOT per imporre la chiusura della finestra
*/
void HistoDraw::closeWindow(){
	_extClose = true;
	this->close();
}

/**
ridefinizione dell'avvento di chiusura della finestra, emetto uno specifico segnale
*/
void HistoDraw::closeEvent(QCloseEvent *event){
	
	//CASO PREMO CHIUDI FINESTRA
	if(!_extClose){
		
		//qDebug() << "VIDEO - WINDOW CLOSING INT";
		
		emit Xpressed();
		event->ignore();
	}
	else{
		
		//qDebug() << "VIDEO - WINDOW CLOSING ext";
		
		emit windowClosing();
	}
	_extClose = false;
}
