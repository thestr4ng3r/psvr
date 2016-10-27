
#ifndef PSVR_HMDWIDGET_H
#define PSVR_HMDWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class HMDWidget : public QOpenGLWidget
{
	Q_OBJECT

	public:
		HMDWidget(QWidget *parent = 0);
		~HMDWidget();

	protected:
		void initializeGL() Q_DECL_OVERRIDE;
		void resizeGL(int w, int h) Q_DECL_OVERRIDE;
		void paintGL() Q_DECL_OVERRIDE;

	private:
		QOpenGLShaderProgram *test_shader;

		QOpenGLBuffer vbo;
		QOpenGLVertexArrayObject vao;

		void RenderEye(int eye);

};


#endif //PSVR_HMDWIDGET_H
