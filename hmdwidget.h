
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

		QOpenGLShaderProgram *test_shader;
		GLint modelview_projection_uni;

		QOpenGLBuffer vbo;
		QOpenGLVertexArrayObject vao;

		QOpenGLTexture *video_tex;


		void RenderEye(int eye, int width, int height);

	public:
		HMDWidget(VideoPlayer *video_player, PSVR *psvr, QWidget *parent = 0);
		~HMDWidget();

	protected:
		void initializeGL() Q_DECL_OVERRIDE;
		void resizeGL(int w, int h) Q_DECL_OVERRIDE;
		void paintGL() Q_DECL_OVERRIDE;
};


#endif //PSVR_HMDWIDGET_H
