
#ifndef PSVR_HMDWIDGET_H
#define PSVR_HMDWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>

#include "videoplayer.h"
#include "psvr.h"

class HMDWidget : public QOpenGLWidget
{
	Q_OBJECT

	private:
		VideoPlayer *video_player;
		PSVR *psvr;


		QOpenGLFunctions *gl;

		QOpenGLShaderProgram *sphere_shader;
		QOpenGLShaderProgram *distortion_shader;

		QOpenGLBuffer cube_vbo;
		QOpenGLVertexArrayObject cube_vao;

		QOpenGLTexture *video_tex;


		/*QOpenGLBuffer screen_vbo;
		QOpenGLVertexArrayObject screen_vao;
		QOpenGLFramebufferObject *fbo;*/

		float fov;
		float barrel_power;

		//void CreateFBO(int width, int height);
		void UpdateTexture();
		void RenderEye(int eye);

	public:
		HMDWidget(VideoPlayer *video_player, PSVR *psvr, QWidget *parent = 0);
		~HMDWidget();

		float GetFOV()						{ return fov; }
		void SetFOV(float fov)				{ this->fov = fov; }

	protected:
		void initializeGL() Q_DECL_OVERRIDE;
		void resizeGL(int w, int h) Q_DECL_OVERRIDE;
		void paintGL() Q_DECL_OVERRIDE;
};


#endif //PSVR_HMDWIDGET_H
