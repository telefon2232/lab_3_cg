#include "ShaderWidget.h"

ShaderWidget::ShaderWidget(QWidget* parent) :QOpenGLWidget(parent), vert_data_location(0){
	vert_data = new GLfloat[12];//инициализируем массив вершин
	//заполняем его координатами вершин квада (-1,-1, 0), (1,-1, 0), (1,1, 0), (-1,1, 0)
	vert_data[0] = -1.0f;  
	vert_data[1] = -1.0f;  
	vert_data[2] = 0.0f;
	vert_data[3] = 1.0f;  
	vert_data[4] = -1.0f;  
	vert_data[5] = 0.0f;
	vert_data[6] = 1.0f;  
	vert_data[7] = 1.0f;  
	vert_data[8] = 0.0f;
	vert_data[9] = -1.0f;  
	vert_data[10] = 1.0f;  
	vert_data[11] = 0.0f;
};

ShaderWidget::~ShaderWidget(){
	delete[] vert_data;
};

void ShaderWidget::initializeGL(){//компилируем шейдеры и компонуем в них шейдерную программу
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	QOpenGLShader vShader(QOpenGLShader::Vertex);//создаем объект, позволяющий загружать исходный код шейдера с диска и компилировать его(vertex-тип шейдера)
	vShader.compileSourceFile("lab3.vert");//указываем файл шейдера

	QOpenGLShader fShader(QOpenGLShader::Fragment);//аналогично для фрагментного
	fShader.compileSourceFile("lab3.frag");

	//Добавление скомпилированных шейдеров к шейдерной программе
	m_program.addShader(&vShader);
	m_program.addShader(&fShader);

	//link - компонует добавленные шейдеры в шейдерную программу и возвращает true, если компановка прошла успешно
	//это нужно для стыковки выходных данных одного шейдера с входными данными другого шейдера
	//и для стыковки входных/выходных переменных шейдеров с соответствующими областями памяти в окружении opengl
	if (!m_program.link()){
		qWarning("Error link program shader");
		return;
	}

	qDebug() << QString("Log program");
	qDebug() << m_program.log();

	//получаем расположение массива вершин  в списке параметров шейдерной программы, если не находим, возвращаем -1
	vert_data_location = m_program.attributeLocation("vertex");

	if (!m_program.bind())//активируем шейдерную программу в текущем контексте qopenglcontext
		qWarning("Error bind program shader");

	m_program.setUniformValue("camera.position", QVector3D(0.0, -1.0, -9));//по осям двигаем
	m_program.setUniformValue("camera.view", QVector3D(0.0, 0.0, 1.0));//увеличение 
	m_program.setUniformValue("camera.up", QVector3D(0.0, 1.0, 0.0));
	m_program.setUniformValue("camera.side", QVector3D(1.0, 0.0, 0.0));

	m_program.setUniformValue("spheres[0].Centre", QVector3D(-1.0, -1.0, -2.0));
	m_program.setUniformValue("spheres[0].Radius", 1.5f);
	m_program.setUniformValue("spheres[0].Color", QVector3D(4.0, 1.0, 0.0));

	m_program.setUniformValue("spheres[1].Centre", QVector3D(2.0, 1.0, 2.0));
	m_program.setUniformValue("spheres[1].Radius", 1.0f);
	m_program.setUniformValue("spheres[1].Color", QVector3D(0.0, 1.0, 0.0));

	m_program.setUniformValue("trianglein[0].v1", QVector3D(-10.0, -5.0, 5.0));
	m_program.setUniformValue("trianglein[0].v2", QVector3D(-6.0, -5.0, 5.0));
	m_program.setUniformValue("trianglein[0].v3", QVector3D(-6.0, -5.0, 1.54));
	m_program.setUniformValue("trianglein[0].color", QVector3D(0.0, 1.0, 1.0));
	m_program.setUniformValue("trianglein[1].v1", QVector3D(-10.0, -5.0, 5.0));
	m_program.setUniformValue("trianglein[1].v2", QVector3D(-6.0, -5.0, 5.0));
	m_program.setUniformValue("trianglein[1].v3", QVector3D(-6.0, 0.0, 2.0));//верхушка
	m_program.setUniformValue("trianglein[1].color", QVector3D(0.0, 1.0, 1.0));
	m_program.setUniformValue("trianglein[2].v1", QVector3D(-10.0, -5.0, 5.0));
	m_program.setUniformValue("trianglein[2].v2", QVector3D(-6.0, -5.0, 1.54));
	m_program.setUniformValue("trianglein[2].v3", QVector3D(-6.0, 0.0, 2.0));//верхушка
	m_program.setUniformValue("trianglein[2].color", QVector3D(0.0, 1.0, 1.0));
	m_program.setUniformValue("trianglein[3].v1", QVector3D(-6.0, -5.0, 5.0));
	m_program.setUniformValue("trianglein[3].v2", QVector3D(-6.0, -5.0, 1.54));
	m_program.setUniformValue("trianglein[3].v3", QVector3D(-6.0, 0.0, 2.0));//верхушка
	m_program.setUniformValue("trianglein[3].color", QVector3D(0.0, 1.0, 1.0));

	m_program.setUniformValue("scale", QVector2D(width(), height()));//устанавливает значение в единую переменную с именем scale в текущем контексте

	m_program.release();//выпускает активную шейдерную программу
};

void ShaderWidget::resizeGL(int newWidth, int newHeight){
	glViewport(0, 0, newWidth, newHeight);//задаем параметры окна
	if (!m_program.bind())//активируем шейдерную программу в текущем контексте qopenglcontext
		qWarning("Error bind program shader");
	m_program.setUniformValue("scale", QVector2D(width(), height()));//устанавливает значение в единую переменную с именем scale в текущем контексте
	m_program.release();//выпускает активную шейдерную программу
};

void ShaderWidget::paintGL(){
	glClear(GL_COLOR_BUFFER_BIT);//очищаем буфер, которые в настоящее время включены для цветовой записи
	if (!m_program.bind())//активируем шейдерную программу в текущем контексте qopenglcontext
		return;
	m_program.enableAttributeArray(vert_data_location);//разрешаем использование массива вершин для указанной переменной в шейдерной программе
	m_program.setAttributeArray(vert_data_location, vert_data, 3);//задаем массив вершин в шейдерной программе
	glDrawArrays(GL_QUADS, 0, 4);//визуализация примитива из данных массива(тип примитива, индекс вершины, с которой будем выводить объект, количество вершин)
	m_program.disableAttributeArray(vert_data_location);//отключение массива вершин
	m_program.release();//выпускает активную шейдерную программу
};