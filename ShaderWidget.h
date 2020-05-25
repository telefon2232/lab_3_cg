#ifndef SHADERWIDGET_H
#define SHADERWIDGET_H
#include <qtopengl>
#include <qopenglwidget.h>
#include <qimage.h>
#include <algorithm>

class ShaderWidget : public QOpenGLWidget
{
protected:
	void initializeGL() override;//компилируем шейдеры и компонуем в них шейдерную программу
	void resizeGL(int newWidth, int newHeight) override;
	void paintGL() override;

public:
	ShaderWidget(QWidget* parent = 0);
	~ShaderWidget();

private:
	QOpenGLShaderProgram m_program;//подключение шейдеров
	GLfloat* vert_data;//массив вершин
	int vert_data_location;//целое число,в котором мы храним расположение этого массива в пределах списка параметров шейдерной программы
};

#endif
