#include "main_window.hpp"
#include "ui_main_window.h"
#include "output_window.hpp"

#include "simulation/integrators/euler.hpp"	
#include "simulation/integrators/verlet.hpp"	
#include "simulation/integrators/rk4.hpp"	
#include "simulation/math_obj.hpp"
#include "simulation/satellite.hpp"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QStandardPaths>
#include <QString>
#include <QWidget>

#include <exception>
#include <fstream>
#include <stdexcept>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow),
	  sim_data(orbsim::SimData{0, nullptr, nullptr, nullptr}) {

	ui->setupUi(this);

	connect(ui->actionExport, &QAction::triggered,
			this, &MainWindow::export_data);


	connect(ui->ChooseInitCond, &QComboBox::currentIndexChanged,
			this, &MainWindow::update_init_cond);

	connect(ui->StartTimeSpinBox, &QSpinBox::valueChanged,
			this, [this](int t_start) {
				try {
					this->sat.set_t_start(t_start);
				} catch (const std::exception &e) {
					QMessageBox err_msg;
					err_msg.setText(e.what());
					err_msg.exec();
				}
			});

	connect(ui->EndTimeSpinBox, &QSpinBox::valueChanged,
			this, [this](int t_end) {
				try {
					this->sat.set_t_end(t_end);
				} catch (const std::exception &e) {
					QMessageBox err_msg;
					err_msg.setText(e.what());
					err_msg.exec();
				}
			});

	connect(ui->TimeStepsSpinBox, &QSpinBox::valueChanged,
			this, [this](int t_steps) { this->sat.set_t_steps(t_steps); });

	connect(ui->ChooseIntegrator, &QComboBox::currentTextChanged,
			this, [this](QString integ_name) { this->sat.set_integ(integ_name.toStdString()); });

	connect(ui->SimulateButton, &QPushButton::clicked,
			this, &MainWindow::simulate);
	
	connect(this, &MainWindow::new_sim_data,
			findChild<OutputWindow *>("outputWindow"), &OutputWindow::update_sim_data);

	load_example_values();
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::export_data() {

	if (this->sim_data.steps == 0) {
		QMessageBox err_msg;
		err_msg.setText("No simulation data found!");
		err_msg.exec();
		return;
	}

	QString file_path = QFileDialog::getSaveFileName(this,
		tr("Export Simulated Data"),
		QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
		tr("Text Files (*.txt)")
	);

	std::string output;
	for (int i = 0; i < sim_data.steps - 1; i++) {
		output += sim_data.pos_arr[i].to_str() + " " + sim_data.vel_arr[i].to_str() + "\n";
	}

	std::ofstream o(file_path.toStdString());
	o << output;
}

void MainWindow::simulate() {

	// this is a bit dumb, but we could fix it in the future
	switch (ui->ChooseInitCond->currentIndex()) {
	case 0:
		this->sat.set_cart_elem(orbsim::CartElem{
			orbsim::Vec3{
				ui->PosSpinBoxX->value(),
				ui->PosSpinBoxY->value(),
				ui->PosSpinBoxZ->value()
			},
			orbsim::Vec3{
				ui->VelSpinBoxX->value(),
				ui->VelSpinBoxY->value(),
				ui->VelSpinBoxZ->value()
			}
		});
		break;
	case 1:
		this->sat.set_kepl_elem(orbsim::KeplElem{
			ui->EccSpinBox->value(),
			ui->SemMajAxSpinBox->value(),
			ui->IncSpinBox->value(),
			ui->RiAscNodeSpinBox->value(),
			ui->ArgOfPerSpinBox->value(),
			ui->TrueAnomSpinBox->value()
		});
		break;
	}

	this->sim_data = this->sat.propagate();

	QString output;
	for (int i = 0; i < sim_data.steps - 1; i++) {
		output += sim_data.pos_arr[i].to_str() + " " + sim_data.vel_arr[i].to_str() + "\n";
	}

	ui->OutputConsole->setText(output);
	emit new_sim_data(sim_data);
}

void MainWindow::load_example_values() {

	this->sat = orbsim::Satellite();

	sync_cart_gui();
	sync_kepl_gui();
	ui->StartTimeSpinBox->setValue(this->sat.get_t_start());
	ui->EndTimeSpinBox->setValue(this->sat.get_t_end());
	ui->TimeStepsSpinBox->setValue(this->sat.get_t_steps());
	ui->ChooseIntegrator->setCurrentText(this->sat.get_integ_name().c_str());
}

void MainWindow::update_init_cond(int init_cond_index) {
	switch (init_cond_index) {
	case 0:
		this->sat.set_kepl_elem(orbsim::KeplElem{
			ui->EccSpinBox->value(),
			ui->SemMajAxSpinBox->value(),
			ui->IncSpinBox->value(),
			ui->RiAscNodeSpinBox->value(),
			ui->ArgOfPerSpinBox->value(),
			ui->TrueAnomSpinBox->value()
		});
		sync_cart_gui();
		break;
	case 1:
		this->sat.set_cart_elem(orbsim::CartElem{
			orbsim::Vec3{
				ui->PosSpinBoxX->value(),
				ui->PosSpinBoxY->value(),
				ui->PosSpinBoxZ->value()
			},
			orbsim::Vec3{
				ui->VelSpinBoxX->value(),
				ui->VelSpinBoxY->value(),
				ui->VelSpinBoxZ->value()
			}
		});
		sync_kepl_gui();
		break;
	}
}

void MainWindow::sync_cart_gui() {
	ui->PosSpinBoxX->setValue(this->sat.get_cart_elem().pos.x);
	ui->PosSpinBoxY->setValue(this->sat.get_cart_elem().pos.y);
	ui->PosSpinBoxZ->setValue(this->sat.get_cart_elem().pos.z);

	ui->VelSpinBoxX->setValue(this->sat.get_cart_elem().vel.x);
	ui->VelSpinBoxY->setValue(this->sat.get_cart_elem().vel.y);
	ui->VelSpinBoxZ->setValue(this->sat.get_cart_elem().vel.z);
}

void MainWindow::sync_kepl_gui() {
	ui->EccSpinBox->setValue(this->sat.get_kepl_elem().ecc);
	ui->SemMajAxSpinBox->setValue(this->sat.get_kepl_elem().sem_maj_ax);
	ui->IncSpinBox->setValue(this->sat.get_kepl_elem().inc);
	ui->RiAscNodeSpinBox->setValue(this->sat.get_kepl_elem().ri_asc_node);
	ui->ArgOfPerSpinBox->setValue(this->sat.get_kepl_elem().arg_of_per);
	ui->TrueAnomSpinBox->setValue(this->sat.get_kepl_elem().true_anom);
}
