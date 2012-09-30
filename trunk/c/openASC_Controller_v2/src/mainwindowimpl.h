#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "rotatordialog.h"
#include "settingsdialog.h"
#include "terminaldialog.h"
#include "tcpclass.h"
#include "errordialog.h"

#include <QtNetwork>
#include <QTcpSocket>
#include <QTimer>
#include <QImage>
#include <QProgressBar>
#include <QCheckBox>

typedef struct {
	unsigned char currentBand;
	unsigned char currentAntennas;
	unsigned char currentRXAntennas;
	unsigned int currentErrors;
	unsigned char subMenuType[4];
	unsigned char antennaFlags[4];
	unsigned char antSubOptSelected[4];
//struct_sub_menu_array subMenuArray;
//	struct_sub_menu_stack subMenuStack;
} status_struct;

typedef struct {
	unsigned char combCount;
	QString combNames[6];
} struct_sub_menu_stack;

class MainWindowImpl : public QMainWindow, public Ui::MainWindowImpl {
Q_OBJECT
public:
		MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
		RotatorDialog *rotatorWindow;
		SettingsDialog *settingsDialog;
		terminalDialog *terminalWindow;
		ErrorDialog *errorDialog;
		void pushButtonPressed(unsigned char button);
		QString getBandName(int bandIndex);
		unsigned char glcd_buffer[8][128];
		TCPClass *TCPComm;
		void updateDisplay();
private:
		int interfaceType;
		QTimer *timerPollRXQueue;
		QTimer *timerPollStatus;
		status_struct status;
		unsigned char bandChangedFlag;
protected:
		void paintEvent(QPaintEvent *event);
private slots:
public slots:
		void actionRebootTriggered();
		void WindowRotatorsTriggered();
		void actionConnectTriggered();
		void actionDisconnectTriggered();
		void actionSettingsEditTriggered();
		void actionErrorDialogTriggered();
		void actionTerminalTriggered();

		void pushButtonTX1Clicked();
		void pushButtonTX2Clicked();
		void pushButtonTX3Clicked();
		void pushButtonTX4Clicked();

		void comboBoxBandIndexChanged(int index);

		void pushButtonRXAnt1Clicked();
		void pushButtonRXAnt2Clicked();
		void pushButtonRXAnt3Clicked();
		void pushButtonRXAnt4Clicked();
		void pushButtonRXAnt5Clicked();
		void pushButtonRXAnt6Clicked();
		void pushButtonRXAnt7Clicked();
		void pushButtonRXAnt8Clicked();
		void pushButtonRXAnt9Clicked();
		void pushButtonRXAnt10Clicked();

		void pushButtonRXAntClicked();

		void timerPollRXQueueUpdate();
		void timerPollStatusUpdate();

		void pushButtonMenuClicked();
		void pushButtonMenuEnterClicked();
		void pushButtonMenuLeftClicked();
		void pushButtonMenuRightClicked();

		void pushButtonSubClicked();
};
#endif




