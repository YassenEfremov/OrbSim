#include "simulation/satellite.hpp"

#include <QMainWindow>
#include <QString>
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

	void update_init_cond(int init_cond_index);

	void sync_cart_gui();
	void sync_kepl_gui();

private:
	Ui::MainWindow *ui;

	orbsim::Satellite sat;
};

