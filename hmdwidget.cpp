
#include "hmdwidget.h"

#include <QOpenGLContext>
#include <QOpenGLTexture>

#include <GL/gl.h>
#include <GL/glu.h>

HMDWidget::HMDWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	video_player = 0;
}

HMDWidget::~HMDWidget()
{
	delete test_shader;
}

void HMDWidget::SetVideoPlayer(VideoPlayer *video_player)
{
	this->video_player = video_player;
}

static const QVector3D cube_vertices[] = {
		// back
		QVector3D( -1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D( -1.0f, -1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D(  1.0f, -1.0f, -1.0f), QVector3D(0.0f, 0.0f, 1.0f),

		QVector3D(  1.0f, -1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D(  1.0f,  1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D( -1.0f,  1.0f, -1.0f), QVector3D(0.0f, 0.0f, 1.0f),

		// front
		QVector3D(  1.0f,  1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D(  1.0f, -1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D( -1.0f, -1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f),

		QVector3D( -1.0f, -1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D( -1.0f,  1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D(  1.0f,  1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f),

		// left
		QVector3D( -1.0f,  1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D( -1.0f, -1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D( -1.0f, -1.0f, -1.0f), QVector3D(0.0f, 0.0f, 1.0f),

		QVector3D( -1.0f, -1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D( -1.0f,  1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D( -1.0f,  1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f),

		// right
		QVector3D(  1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D(  1.0f, -1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D(  1.0f, -1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f),

		QVector3D(  1.0f, -1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D(  1.0f,  1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D(  1.0f,  1.0f, -1.0f), QVector3D(0.0f, 0.0f, 1.0f),

		// top
		QVector3D( -1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D(  1.0f,  1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D(  1.0f,  1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f),

		QVector3D(  1.0f,  1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D( -1.0f,  1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D( -1.0f,  1.0f, -1.0f), QVector3D(0.0f, 0.0f, 1.0f),

		// bottom
		QVector3D(  1.0f, -1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D( -1.0f, -1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D( -1.0f, -1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f),

		QVector3D( -1.0f, -1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D(  1.0f, -1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D(  1.0f, -1.0f, -1.0f), QVector3D(0.0f, 0.0f, 1.0f)
	};


static const QVector3D plane_vertices[] = {
		QVector3D( -1.0f,  1.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D( -1.0f, -1.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f),
		QVector3D(  1.0f, -1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f),

		QVector3D(  1.0f, -1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D(  1.0f,  1.0f, 0.0f), QVector3D(1.0f, 1.0f, 0.0f),
		QVector3D( -1.0f,  1.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)
	};

void HMDWidget::initializeGL()
{
	test_shader = new QOpenGLShaderProgram(this);
	test_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shader/test.vert");
	test_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shader/test.frag");
	test_shader->link();

	test_shader->bind();
	test_shader->bindAttributeLocation("vertex_attr", 0);
	test_shader->bindAttributeLocation("uv_attr", 1);
	//modelview_projection_uni = test_shader->uniformLocation("modelview_projection_uni");

	vbo.create();
	vbo.bind();
	vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo.allocate(plane_vertices, sizeof(plane_vertices));

	vao.create();
	vao.bind();
	test_shader->enableAttributeArray(0);
	test_shader->enableAttributeArray(1);
	test_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D) * 2);
	test_shader->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(QVector3D) * 2);

	test_shader->release();
	vbo.release();
	vao.release();

	int width = 512;
	int height = 512;

	/*video_tex = new QOpenGLTexture(QOpenGLTexture::Target2D);
	video_tex->setMinificationFilter(QOpenGLTexture::Nearest);
	video_tex->setMagnificationFilter(QOpenGLTexture::Nearest);
	video_tex->create();
	//video_tex->bind();
	video_tex->setFormat(QOpenGLTexture::RGB8U);
	//video_tex->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::UInt8);
	video_tex->setSize(width, height, 1);
	video_tex->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::UInt8);

	unsigned char *data = new unsigned char[width * height * 3];
	for(int x=0; x<width; x++)
	{
		for(int y=0; y<height; y++)
		{
			float v = fabsf(((float)x / (float)width) * 2.0f - 1.0f)
					  * fabsf(((float)y / (float)height) * 2.0f - 1.0f);
			unsigned char u = (unsigned char)(v * 255.0f);
			data[y*width + x] = 255;
			data[y*width + x + 1] = u;
			data[y*width + x + 2] = u;
		}
	}
	video_tex->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (void *)data);
	delete[] data;*/

	QImage image;
	image.load("test.png");
	video_tex = new QOpenGLTexture(image);
}


void HMDWidget::resizeGL(int w, int h)
{
}

void HMDWidget::paintGL()
{
	int w = width();
	int h = height();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, w, h);
	RenderEye(0);

	//glViewport(w/2, 0, w/2, h);
	//RenderEye(1);
}

void HMDWidget::RenderEye(int eye)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	test_shader->bind();

	QMatrix4x4 modelview_matrix;
	modelview_matrix.lookAt(QVector3D(1.0f, 1.0f, 2.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));

	QMatrix4x4 projection_matrix;
	projection_matrix.perspective(80.0f, (float)width() / (float)height(), 0.1f, 100.0f);
	test_shader->setUniformValue("modelview_projection_uni", projection_matrix * modelview_matrix);

	glActiveTexture(GL_TEXTURE0);
	test_shader->setUniformValue("tex_uni", 0);
	video_tex->bind(0);

	vao.bind();
	glDrawArrays(GL_TRIANGLES, 0, 6*1);
	vao.release();
	test_shader->release();
}