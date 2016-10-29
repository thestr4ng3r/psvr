
#include "hmdwidget.h"

HMDWidget::HMDWidget(VideoPlayer *video_player, PSVR *psvr, QWidget *parent) : QOpenGLWidget(parent)
{
	this->video_player = video_player;
	this->psvr = psvr;

	gl = 0;

	test_shader = 0;
}

HMDWidget::~HMDWidget()
{
	delete test_shader;
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
		QVector3D( -1.0f,  1.0f, -1.0f), QVector3D(0.0f, 0.0f, 0.0f),
		QVector3D( -1.0f, -1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f),
		QVector3D(  1.0f, -1.0f, -1.0f), QVector3D(1.0f, 1.0f, 0.0f),

		QVector3D(  1.0f, -1.0f, -1.0f), QVector3D(1.0f, 1.0f, 0.0f),
		QVector3D(  1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f),
		QVector3D( -1.0f,  1.0f, -1.0f), QVector3D(0.0f, 0.0f, 0.0f)
	};

void HMDWidget::initializeGL()
{
	gl = context()->functions();

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

	video_tex = new QOpenGLTexture(QOpenGLTexture::Target2D);
	video_tex->create();
	video_tex->setFormat(QOpenGLTexture::RGB8_UNorm);
	video_tex->setSize(width, height);
	video_tex->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
	video_tex->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::PixelType::UInt8);
	video_tex->bind();

	unsigned char *data = new unsigned char[width * height * 3];
	for(int x=0; x<width; x++)
	{
		for(int y=0; y<height; y++)
		{
			data[(y*width + x) * 3 + 0] = 255;
			data[(y*width + x) * 3 + 1] = 0;
			data[(y*width + x) * 3 + 2] = 0;
		}
	}
	video_tex->setData(QOpenGLTexture::PixelFormat::RGB, QOpenGLTexture::PixelType::UInt8, (const void *)data);
	delete[] data;
}


void HMDWidget::resizeGL(int w, int h)
{
	update();
}

void HMDWidget::paintGL()
{
	int w = width();
	int h = height();

	if(video_player)
		video_tex->setData(QOpenGLTexture::PixelFormat::RGB, QOpenGLTexture::PixelType::UInt8, video_player->GetVideoData());

	gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*gl->glViewport(0, 0, w, h);
	RenderEye(0, w, h);*/

	gl->glViewport(0, 0, w/2, h);
	RenderEye(0, w/2, h);

	gl->glViewport(w/2, 0, w/2, h);
	RenderEye(1, w/2, h);

	update();
}

void HMDWidget::RenderEye(int eye, int width, int height)
{
	gl->glEnable(GL_CULL_FACE);
	gl->glEnable(GL_DEPTH_TEST);

	test_shader->bind();

	QMatrix4x4 modelview_matrix;
	//modelview_matrix.lookAt(QVector3D(1.0f, 1.0f, 2.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
	modelview_matrix.rotate(psvr->GetRotationZ(), 0.0f, 0.0f, 1.0f);
	modelview_matrix.rotate(-psvr->GetRotationY(), 1.0f, 0.0f, 0.0f);
	modelview_matrix.rotate(-psvr->GetRotationX(), 0.0f, 1.0f, 0.0f);

	QMatrix4x4 projection_matrix;
	projection_matrix.perspective(80.0f, (float)width / (float)height, 0.1f, 100.0f);
	test_shader->setUniformValue("modelview_projection_uni", projection_matrix * modelview_matrix);

	test_shader->setUniformValue("tex_uni", 0);
	video_tex->bind(0);

	vao.bind();
	gl->glDrawArrays(GL_TRIANGLES, 0, 6*1);
	vao.release();
	test_shader->release();
}
