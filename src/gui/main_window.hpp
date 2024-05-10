#include <QMainWindow>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui {

class MainWindow;

};

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void simulate();
	void load_example_values();

private:
	Ui::MainWindow *ui;
};

