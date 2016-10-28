
#ifndef PSVR_HMDWIDGET_H
#define PSVR_HMDWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include "videoplayer.h"

class HMDWidget : public QOpenGLWidget
{
	Q_OBJECT

	public:
		HMDWidget(QWidget *parent = 0);
		~HMDWidget();

		void SetVideoPlayer(VideoPlayer *video_player);

	protected:
		void initializeGL() Q_DECL_OVERRIDE;
		void resizeGL(int w, int h) Q_DECL_OVERRIDE;
		void paintGL() Q_DECL_OVERRIDE;

	private:
		QOpenGLShaderProgram *test_shader;
		GLint modelview_projection_uni;

		QOpenGLBuffer vbo;
		QOpenGLVertexArrayObject vao;

		QOpenGLTexture *video_tex;


		VideoPlayer *video_player;


		void RenderEye(int eye);

};


#endif //PSVR_HMDWIDGET_H
