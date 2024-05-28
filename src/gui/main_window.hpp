#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "simulation/satellite.hpp"

#include <QMainWindow>
#include <QString>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui {

class MainWindow;

};
QT_END_NAMESPACE

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

signals:
	void new_sim_data(orbsim::SimData new_data);

private:
	Ui::MainWindow *ui;

	orbsim::Satellite sat;
};


#endif	// MAIN_WINDOW_HPP
