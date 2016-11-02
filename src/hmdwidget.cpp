/*
 * Created by Florian Märkl <info@florianmaerkl.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "hmdwidget.h"

HMDWidget::HMDWidget(VideoPlayer *video_player, PSVR *psvr, QWidget *parent) : QOpenGLWidget(parent)
{
	this->video_player = video_player;
	this->psvr = psvr;

	gl = 0;
	//fbo = 0;

	sphere_shader = 0;
	distortion_shader = 0;
	video_tex = 0;

	fov = 80.0f;

	video_angle = 360;
	video_projection_mode = Monoscopic;
	invert_stereo = false;

	rgb_workaround = false;
}

HMDWidget::~HMDWidget()
{
	delete video_tex;
	//delete fbo;
}


static const QVector3D cube_vertices[] = {
		// back
		QVector3D(-1.0f,  1.0f, -1.0f),
		QVector3D(-1.0f, -1.0f, -1.0f),
		QVector3D( 1.0f, -1.0f, -1.0f),

		QVector3D( 1.0f, -1.0f, -1.0f),
		QVector3D( 1.0f,  1.0f, -1.0f),
		QVector3D(-1.0f,  1.0f, -1.0f),

		// front
		QVector3D( 1.0f,  1.0f,  1.0f),
		QVector3D( 1.0f, -1.0f,  1.0f),
		QVector3D(-1.0f, -1.0f,  1.0f),

		QVector3D(-1.0f, -1.0f,  1.0f),
		QVector3D(-1.0f,  1.0f,  1.0f),
		QVector3D( 1.0f,  1.0f,  1.0f),

		// left
		QVector3D(-1.0f,  1.0f,  1.0f),
		QVector3D(-1.0f, -1.0f,  1.0f),
		QVector3D(-1.0f, -1.0f, -1.0f),

		QVector3D(-1.0f, -1.0f, -1.0f),
		QVector3D(-1.0f,  1.0f, -1.0f),
		QVector3D(-1.0f,  1.0f,  1.0f),

		// right
		QVector3D( 1.0f,  1.0f, -1.0f),
		QVector3D( 1.0f, -1.0f, -1.0f),
		QVector3D( 1.0f, -1.0f,  1.0f),

		QVector3D( 1.0f, -1.0f,  1.0f),
		QVector3D( 1.0f,  1.0f,  1.0f),
		QVector3D( 1.0f,  1.0f, -1.0f),

		// top
		QVector3D(-1.0f,  1.0f, -1.0f),
		QVector3D( 1.0f,  1.0f, -1.0f),
		QVector3D( 1.0f,  1.0f,  1.0f),

		QVector3D( 1.0f,  1.0f,  1.0f),
		QVector3D(-1.0f,  1.0f,  1.0f),
		QVector3D(-1.0f,  1.0f, -1.0f),

		// bottom
		QVector3D( 1.0f, -1.0f, -1.0f),
		QVector3D(-1.0f, -1.0f, -1.0f),
		QVector3D(-1.0f, -1.0f,  1.0f),

		QVector3D(-1.0f, -1.0f,  1.0f),
		QVector3D( 1.0f, -1.0f,  1.0f),
		QVector3D( 1.0f, -1.0f, -1.0f)
	};

static const QVector2D screen_vertices[] = {
		QVector2D(-1.0f,  1.0f),
		QVector2D(-1.0f, -1.0f),
		QVector2D( 1.0f,  1.0f),
		QVector2D( 1.0f, -1.0f)
	};

void HMDWidget::initializeGL()
{
	gl = context()->functions();

	sphere_shader = new QOpenGLShaderProgram(this);
	sphere_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/sphere.vert");
	sphere_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/sphere.frag");
	sphere_shader->link();

	sphere_shader->bind();
	sphere_shader->bindAttributeLocation("vertex_attr", 0);

	cube_vbo.create();
	cube_vbo.bind();
	cube_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	cube_vbo.allocate(cube_vertices, sizeof(cube_vertices));

	cube_vao.create();
	cube_vao.bind();
	sphere_shader->enableAttributeArray(0);
	sphere_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3);

	sphere_shader->release();
	cube_vbo.release();
	cube_vao.release();

	video_tex = new QOpenGLTexture(QOpenGLTexture::Target2D);
	video_tex->create();
	video_tex->setFormat(QOpenGLTexture::RGB8_UNorm);
	video_tex->setSize(1, 1);
	video_tex->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
	video_tex->allocateStorage(rgb_workaround ? QOpenGLTexture::RGB : QOpenGLTexture::BGR, QOpenGLTexture::PixelType::UInt8);
	video_tex->bind();
	unsigned char data[3] = { 0, 0, 0};
	video_tex->setData(rgb_workaround ? QOpenGLTexture::RGB : QOpenGLTexture::BGR, QOpenGLTexture::PixelType::UInt8, (const void *)data);


	/*distortion_shader = new QOpenGLShaderProgram(this);
	distortion_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shader/distortion.vert");
	distortion_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shader/distortion.frag");
	distortion_shader->link();

	distortion_shader->bind();
	distortion_shader->bindAttributeLocation("vertex_attr", 0);

	screen_vbo.create();
	screen_vbo.bind();
	screen_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	screen_vbo.allocate(screen_vertices, sizeof(screen_vertices));

	screen_vao.create();
	screen_vao.bind();
	distortion_shader->enableAttributeArray(0);
	distortion_shader->setAttributeBuffer(0, GL_FLOAT, 0, 2);

	CreateFBO(width() / 2, height());*/
}


void HMDWidget::resizeGL(int w, int h)
{
	//CreateFBO(w / 2, h);
	update();
}

void HMDWidget::paintGL()
{
	int w = width();
	int h = height();

	UpdateTexture();

	gl->glClear(GL_COLOR_BUFFER_BIT);
	gl->glEnable(GL_CULL_FACE);
	gl->glDisable(GL_DEPTH_TEST);

	/*gl->glViewport(0, 0, w, h);
	RenderEye(0, w, h);*/

	RenderEye(0);
	RenderEye(1);

	update();
}


/*void HMDWidget::CreateFBO(int width, int height)
{
	delete fbo;
	fbo = new QOpenGLFramebufferObject(width, height);
}*/

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
		video_tex->allocateStorage(rgb_workaround ? QOpenGLTexture::RGB : QOpenGLTexture::BGR, QOpenGLTexture::PixelType::UInt8);
	}

	video_tex->bind();
	video_tex->setData(rgb_workaround ? QOpenGLTexture::RGB : QOpenGLTexture::BGR, QOpenGLTexture::PixelType::UInt8, video_player->GetVideoData());
}

void HMDWidget::RenderEye(int eye)
{
	int w = width();
	int h = height();

	//fbo->bind();
	//gl->glViewport(0, 0, w/2, h);
	//gl->glClear(GL_COLOR_BUFFER_BIT);

	gl->glViewport(eye == 1 ? w/2 : 0, 0, w/2, h);



	sphere_shader->bind();

	QMatrix4x4 modelview_matrix;
	modelview_matrix = psvr->GetModelViewMatrix();

	QMatrix4x4 projection_matrix;
	projection_matrix.perspective(fov, ((float)(w/2)) / (float)h, 0.1f, 100.0f);
	sphere_shader->setUniformValue("modelview_projection_uni", projection_matrix * modelview_matrix);

	sphere_shader->setUniformValue("tex_uni", 0);
	video_tex->bind(0);

	int eye_inv = invert_stereo ? 1 - eye : eye;

	switch(video_projection_mode)
	{
		case Monoscopic:
			sphere_shader->setUniformValue("min_max_uv_uni", 0.0f, 0.0f, 1.0f, 1.0f);
			break;
		case OverUnder:
			if(eye_inv == 1)
				sphere_shader->setUniformValue("min_max_uv_uni", 0.0f, 0.5f, 1.0f, 1.0f);
			else
				sphere_shader->setUniformValue("min_max_uv_uni", 0.0f, 0.0f, 1.0f, 0.5f);
			break;
		case SideBySide:
			if(eye_inv == 1)
				sphere_shader->setUniformValue("min_max_uv_uni", 0.0f, 0.0f, 0.5f, 1.0f);
			else
				sphere_shader->setUniformValue("min_max_uv_uni", 0.5f, 0.0f, 1.0f, 1.0f);
			break;
	}

	sphere_shader->setUniformValue("projection_angle_factor_uni", 360.0f / (float)video_angle);

	cube_vao.bind();
	gl->glDrawArrays(GL_TRIANGLES, 0, 6*6);
	cube_vao.release();
	sphere_shader->release();



/*QOpenGLFramebufferObject::bindDefault();

	gl->glViewport(eye == 1 ? w/2 : 0, 0, w/2, h);

	distortion_shader->bind();
	distortion_shader->setUniformValue("tex_uni", 0);
	distortion_shader->setUniformValue("barrel_power_uni", barrel_power);

	gl->glActiveTexture(GL_TEXTURE0);
	gl->glBindTexture(GL_TEXTURE_2D, fbo->texture());

	screen_vao.bind();
	gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	screen_vao.release();

	distortion_shader->release();*/
}
