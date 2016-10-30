
#include "hmdwidget.h"

HMDWidget::HMDWidget(VideoPlayer *video_player, PSVR *psvr, QWidget *parent) : QOpenGLWidget(parent)
{
	this->video_player = video_player;
	this->psvr = psvr;

	gl = 0;

	sphere_shader = 0;

	fov = 90.0f;
}

HMDWidget::~HMDWidget()
{
	delete sphere_shader;
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

	sphere_shader = new QOpenGLShaderProgram(this);
	sphere_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shader/sphere.vert");
	sphere_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shader/sphere.frag");
	sphere_shader->link();

	sphere_shader->bind();
	sphere_shader->bindAttributeLocation("vertex_attr", 0);
	sphere_shader->bindAttributeLocation("uv_attr", 1);
	//modelview_projection_uni = sphere_shader->uniformLocation("modelview_projection_uni");

	vbo.create();
	vbo.bind();
	vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo.allocate(cube_vertices, sizeof(cube_vertices));

	vao.create();
	vao.bind();
	sphere_shader->enableAttributeArray(0);
	sphere_shader->enableAttributeArray(1);
	sphere_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D) * 2);
	sphere_shader->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(QVector3D) * 2);

	sphere_shader->release();
	vbo.release();
	vao.release();

	video_tex = new QOpenGLTexture(QOpenGLTexture::Target2D);
	video_tex->create();
	video_tex->setFormat(QOpenGLTexture::RGB8_UNorm);
	video_tex->setSize(1, 1);
	video_tex->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
	video_tex->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::PixelType::UInt8);
	video_tex->bind();
	unsigned char data[3] = { 0, 0, 0};
	video_tex->setData(QOpenGLTexture::PixelFormat::RGB, QOpenGLTexture::PixelType::UInt8, (const void *)data);
}


void HMDWidget::resizeGL(int w, int h)
{
	update();
}

void HMDWidget::paintGL()
{
	int w = width();
	int h = height();

	UpdateTexture();

	gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*gl->glViewport(0, 0, w, h);
	RenderEye(0, w, h);*/

	gl->glViewport(0, 0, w/2, h);
	RenderEye(0, w/2, h);

	gl->glViewport(w/2, 0, w/2, h);
	RenderEye(1, w/2, h);

	update();
}

void HMDWidget::UpdateTexture()
{
	if(!video_player->GetVideoData())
		return;

	if(video_tex->width() != video_player->GetWidth() || video_tex->height() != video_player->GetHeight())
	{
		if(video_tex->isStorageAllocated())
		{
			video_tex->destroy();
			video_tex->create();
		}
		video_tex->setFormat(QOpenGLTexture::RGB8_UNorm);
		video_tex->setSize(video_player->GetWidth(), video_player->GetHeight());
		video_tex->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
		video_tex->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::PixelType::UInt8);
	}

	video_tex->bind();
	video_tex->setData(QOpenGLTexture::PixelFormat::RGB, QOpenGLTexture::PixelType::UInt8, video_player->GetVideoData());
}

void HMDWidget::RenderEye(int eye, int width, int height)
{
	gl->glEnable(GL_CULL_FACE);
	gl->glDisable(GL_DEPTH_TEST);

	sphere_shader->bind();

	QMatrix4x4 modelview_matrix;
	//modelview_matrix.rotate(psvr->GetRotationZ(), 0.0f, 0.0f, 1.0f);
	//modelview_matrix.rotate(-psvr->GetRotationY(), 1.0f, 0.0f, 0.0f);
	//modelview_matrix.rotate(-psvr->GetRotationX(), 0.0f, 1.0f, 0.0f);
	modelview_matrix = psvr->GetModelViewMatrix();

	QMatrix4x4 projection_matrix;
	projection_matrix.perspective(fov, (float)width / (float)height, 0.1f, 100.0f);
	sphere_shader->setUniformValue("modelview_projection_uni", projection_matrix * modelview_matrix);

	sphere_shader->setUniformValue("tex_uni", 0);
	video_tex->bind(0);

	if(eye == 1)
		sphere_shader->setUniformValue("min_max_uv_uni", 0.0f, 0.5f, 1.0f, 1.0f);
	else
		sphere_shader->setUniformValue("min_max_uv_uni", 0.0f, 0.0f, 1.0f, 0.5f);

	vao.bind();
	gl->glDrawArrays(GL_TRIANGLES, 0, 6*6);
	vao.release();
	sphere_shader->release();
}
