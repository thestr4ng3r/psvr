
#ifndef PSVR_HMDWIDGET_H
#define PSVR_HMDWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>

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
		GLint modelview_projection_uni;

		QOpenGLBuffer vbo;
		QOpenGLVertexArrayObject vao;

		QOpenGLTexture *video_tex;

		float fov;

		void UpdateTexture();
		void RenderEye(int eye, int width, int height);

	public:
		HMDWidget(VideoPlayer *video_player, PSVR *psvr, QWidget *parent = 0);
		~HMDWidget();

		float GetFOV()			{ return fov; }
		void SetFOV(float fov)	{ this->fov = fov; }

	protected:
		void initializeGL() Q_DECL_OVERRIDE;
		void resizeGL(int w, int h) Q_DECL_OVERRIDE;
		void paintGL() Q_DECL_OVERRIDE;
};


#endif //PSVR_HMDWIDGET_H
