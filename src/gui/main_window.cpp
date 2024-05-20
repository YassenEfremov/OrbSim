#include "main_window.hpp"
#include "ui_main_window.h"

#include "simulation/integrators/euler.hpp"	
#include "simulation/integrators/verlet.hpp"	
#include "simulation/integrators/rk4.hpp"	
#include "simulation/math_obj.hpp"
#include "simulation/satellite.hpp"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QWidget>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow) {

	ui->setupUi(this);

	connect(ui->ChooseInitCond, &QComboBox::currentIndexChanged,
			this, &MainWindow::update_init_cond);

	connect(ui->StartTimeSpinBox, &QSpinBox::valueChanged,
			this, [this](int t_start) { this->sat.set_t_start(t_start); });

	connect(ui->EndTimeSpinBox, &QSpinBox::valueChanged,
			this, [this](int t_end) { this->sat.set_t_end(t_end); });

	connect(ui->TimeStepsSpinBox, &QSpinBox::valueChanged,
			this, [this](int t_steps) { this->sat.set_t_steps(t_steps); });

	connect(ui->ChooseIntegrator, &QComboBox::currentTextChanged,
			this, [this](QString integ_name) { this->sat.set_integ(integ_name.toStdString()); });

	connect(ui->SimulateButton, &QPushButton::clicked,
			this, &MainWindow::simulate);

	load_example_values();
}

MainWindow::~MainWindow() {
	delete ui;
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

	orbsim::SimData sim_data = this->sat.propagate();

	QString output;
	for (int i = 0; i < sim_data.steps - 1; i++) {
		output += sim_data.pos_arr[i].to_str() + " " + sim_data.vel_arr[i].to_str() + "\n";
	}

	ui->OutputConsole->setText(output);
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
