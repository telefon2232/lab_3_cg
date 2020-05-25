#include "lab3.h"
#include <QtWidgets/QApplication>
#include <QtOpenGL/QGLWidget>
#include "ShaderWidget.h"

int main(int argc, char *argv[]){
	QApplication a(argc, argv);
	ShaderWidget w(NULL);
	w.show();
	return a.exec();
}