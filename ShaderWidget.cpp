#include "ShaderWidget.h"

ShaderWidget::ShaderWidget(QWidget* parent) :QOpenGLWidget(parent), vert_data_location(0){
	vert_data = new GLfloat[12];//�������������� ������ ������
	//��������� ��� ������������ ������ ����� (-1,-1, 0), (1,-1, 0), (1,1, 0), (-1,1, 0)
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

void ShaderWidget::initializeGL(){//����������� ������� � ��������� � ��� ��������� ���������
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	QOpenGLShader vShader(QOpenGLShader::Vertex);//������� ������, ����������� ��������� �������� ��� ������� � ����� � ������������� ���(vertex-��� �������)
	vShader.compileSourceFile("lab3.vert");//��������� ���� �������

	QOpenGLShader fShader(QOpenGLShader::Fragment);//���������� ��� ������������
	fShader.compileSourceFile("lab3.frag");

	//���������� ���������������� �������� � ��������� ���������
	m_program.addShader(&vShader);
	m_program.addShader(&fShader);

	//link - ��������� ����������� ������� � ��������� ��������� � ���������� true, ���� ���������� ������ �������
	//��� ����� ��� �������� �������� ������ ������ ������� � �������� ������� ������� �������
	//� ��� �������� �������/�������� ���������� �������� � ���������������� ��������� ������ � ��������� opengl
	if (!m_program.link()){
		qWarning("Error link program shader");
		return;
	}

	qDebug() << QString("Log program");
	qDebug() << m_program.log();

	//�������� ������������ ������� ������  � ������ ���������� ��������� ���������, ���� �� �������, ���������� -1
	vert_data_location = m_program.attributeLocation("vertex");

	if (!m_program.bind())//���������� ��������� ��������� � ������� ��������� qopenglcontext
		qWarning("Error bind program shader");

	m_program.setUniformValue("camera.position", QVector3D(0.0, -1.0, -9));//�� ���� �������
	m_program.setUniformValue("camera.view", QVector3D(0.0, 0.0, 1.0));//���������� 
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
	m_program.setUniformValue("trianglein[1].v3", QVector3D(-6.0, 0.0, 2.0));//��������
	m_program.setUniformValue("trianglein[1].color", QVector3D(0.0, 1.0, 1.0));
	m_program.setUniformValue("trianglein[2].v1", QVector3D(-10.0, -5.0, 5.0));
	m_program.setUniformValue("trianglein[2].v2", QVector3D(-6.0, -5.0, 1.54));
	m_program.setUniformValue("trianglein[2].v3", QVector3D(-6.0, 0.0, 2.0));//��������
	m_program.setUniformValue("trianglein[2].color", QVector3D(0.0, 1.0, 1.0));
	m_program.setUniformValue("trianglein[3].v1", QVector3D(-6.0, -5.0, 5.0));
	m_program.setUniformValue("trianglein[3].v2", QVector3D(-6.0, -5.0, 1.54));
	m_program.setUniformValue("trianglein[3].v3", QVector3D(-6.0, 0.0, 2.0));//��������
	m_program.setUniformValue("trianglein[3].color", QVector3D(0.0, 1.0, 1.0));

	m_program.setUniformValue("scale", QVector2D(width(), height()));//������������� �������� � ������ ���������� � ������ scale � ������� ���������

	m_program.release();//��������� �������� ��������� ���������
};

void ShaderWidget::resizeGL(int newWidth, int newHeight){
	glViewport(0, 0, newWidth, newHeight);//������ ��������� ����
	if (!m_program.bind())//���������� ��������� ��������� � ������� ��������� qopenglcontext
		qWarning("Error bind program shader");
	m_program.setUniformValue("scale", QVector2D(width(), height()));//������������� �������� � ������ ���������� � ������ scale � ������� ���������
	m_program.release();//��������� �������� ��������� ���������
};

void ShaderWidget::paintGL(){
	glClear(GL_COLOR_BUFFER_BIT);//������� �����, ������� � ��������� ����� �������� ��� �������� ������
	if (!m_program.bind())//���������� ��������� ��������� � ������� ��������� qopenglcontext
		return;
	m_program.enableAttributeArray(vert_data_location);//��������� ������������� ������� ������ ��� ��������� ���������� � ��������� ���������
	m_program.setAttributeArray(vert_data_location, vert_data, 3);//������ ������ ������ � ��������� ���������
	glDrawArrays(GL_QUADS, 0, 4);//������������ ��������� �� ������ �������(��� ���������, ������ �������, � ������� ����� �������� ������, ���������� ������)
	m_program.disableAttributeArray(vert_data_location);//���������� ������� ������
	m_program.release();//��������� �������� ��������� ���������
};