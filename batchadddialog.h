#ifndef BATCHADDDIALOG_H
#define BATCHADDDIALOG_H

#include <QDialog>
#include <QtWidgets>
/*
#include "ui_batchadddialog.h"
class ls :public Ui_BatchAddDialog{};*/


class BatchAddDialog : public QDialog
{
public:

    QVBoxLayout *verticalLayout_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_9;
    QLineEdit *Text;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *start_y;
    QLabel *label_2;
    QSpinBox *start_m;
    QLabel *label_3;
    QSpinBox *start_d;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_5;
    QSpinBox *end_y;
    QLabel *label_6;
    QSpinBox *end_m;
    QLabel *label_7;
    QSpinBox *end_d;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_10;
    QRadioButton *y_button;
    QRadioButton *m_button;
    QRadioButton *w_button;
    QRadioButton *d_button;
    QSpinBox *spinBox_7;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    BatchAddDialog(QWidget *parent):QDialog(parent)
    {
        setupUi(this);
        start_y->setValue(QDateTime::currentDateTime().date().year());
        end_y->setValue(QDateTime::currentDateTime().date().year());
        start_m->setValue(QDateTime::currentDateTime().date().month());
        end_m->setValue(QDateTime::currentDateTime().date().month());
        start_d->setValue(QDateTime::currentDateTime().date().day());
        end_d->setValue(QDateTime::currentDateTime().date().day());
        this->setStyleSheet("BatchAddDialog#BatchAddDialog {border-:2px solid black}");

    }
    ~BatchAddDialog(){}
    void setupUi(QDialog *BatchAddDialog)
    {
        if (BatchAddDialog->objectName().isEmpty())
            BatchAddDialog->setObjectName(QStringLiteral("BatchAddDialog"));
        setWindowFlags(Qt::FramelessWindowHint);
        BatchAddDialog->resize(377, 128);
        verticalLayout_3 = new QVBoxLayout(BatchAddDialog);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        widget = new QWidget(BatchAddDialog);
        widget->setObjectName(QStringLiteral("BatchAddDialogwidget"));
        horizontalLayout_5 = new QHBoxLayout(widget);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_9 = new QLabel(widget);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_3->addWidget(label_9);

        Text = new QLineEdit(widget);
        Text->setObjectName(QStringLiteral("Text"));

        horizontalLayout_3->addWidget(Text);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        start_y = new QSpinBox(widget);
        start_y->setObjectName(QStringLiteral("start_y"));
        start_y->setMinimum(1900);
        start_y->setMaximum(2999);

        horizontalLayout->addWidget(start_y);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        start_m = new QSpinBox(widget);
        start_m->setObjectName(QStringLiteral("start_m"));
        start_m->setMinimum(1);
        start_m->setMaximum(12);

        horizontalLayout->addWidget(start_m);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout->addWidget(label_3);

        start_d = new QSpinBox(widget);
        start_d->setObjectName(QStringLiteral("start_d"));
        start_d->setMinimum(1);
        start_d->setMaximum(31);

        horizontalLayout->addWidget(start_d);

        label_4 = new QLabel(widget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_5 = new QLabel(widget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_2->addWidget(label_5);

        end_y = new QSpinBox(widget);
        end_y->setObjectName(QStringLiteral("end_y"));
        end_y->setMinimum(1900);
        end_y->setMaximum(2999);

        horizontalLayout_2->addWidget(end_y);

        label_6 = new QLabel(widget);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_2->addWidget(label_6);

        end_m = new QSpinBox(widget);
        end_m->setObjectName(QStringLiteral("end_m"));
        end_m->setMinimum(1);
        end_m->setMaximum(12);

        horizontalLayout_2->addWidget(end_m);

        label_7 = new QLabel(widget);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_2->addWidget(label_7);

        end_d = new QSpinBox(widget);
        end_d->setObjectName(QStringLiteral("end_d"));
        end_d->setMinimum(1);
        end_d->setMaximum(31);

        horizontalLayout_2->addWidget(end_d);

        label_8 = new QLabel(widget);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_2->addWidget(label_8);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_10 = new QLabel(widget);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_4->addWidget(label_10);

        y_button = new QRadioButton(widget);
        y_button->setObjectName(QStringLiteral("y_button"));

        horizontalLayout_4->addWidget(y_button);

        m_button = new QRadioButton(widget);
        m_button->setObjectName(QStringLiteral("m_button"));

        horizontalLayout_4->addWidget(m_button);

        w_button = new QRadioButton(widget);
        w_button->setObjectName(QStringLiteral("w_button"));

        horizontalLayout_4->addWidget(w_button);

        d_button = new QRadioButton(widget);
        d_button->setObjectName(QStringLiteral("d_button"));

        horizontalLayout_4->addWidget(d_button);

        spinBox_7 = new QSpinBox(widget);
        spinBox_7->setObjectName(QStringLiteral("spinBox_7"));
        spinBox_7->setMinimum(1);
        spinBox_7->setMaximum(9999);

        horizontalLayout_4->addWidget(spinBox_7);


        verticalLayout->addLayout(horizontalLayout_4);


        horizontalLayout_5->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout_2->addWidget(pushButton);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        verticalLayout_2->addWidget(pushButton_2);


        horizontalLayout_5->addLayout(verticalLayout_2);


        verticalLayout_3->addWidget(widget);


        retranslateUi(BatchAddDialog);
        QObject::connect(pushButton, SIGNAL(clicked()), BatchAddDialog, SLOT(accept()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), BatchAddDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(BatchAddDialog);
    }

    void retranslateUi(QDialog *BatchAddDialog)
    {
        BatchAddDialog->setWindowTitle(QApplication::translate("BatchAddDialog", "Dialog", 0));
        label_9->setText(QApplication::translate("BatchAddDialog", "\346\226\207\346\234\254\357\274\232", 0));
        label->setText(QApplication::translate("BatchAddDialog", "\350\265\267\345\247\213\346\227\245\346\234\237\357\274\232", 0));
        label_2->setText(QApplication::translate("BatchAddDialog", "\345\271\264", 0));
        label_3->setText(QApplication::translate("BatchAddDialog", "\346\234\210", 0));
        label_4->setText(QApplication::translate("BatchAddDialog", "\346\227\245", 0));
        label_5->setText(QApplication::translate("BatchAddDialog", "\347\273\210\346\255\242\346\227\245\346\234\237\357\274\232", 0));
        label_6->setText(QApplication::translate("BatchAddDialog", "\345\271\264", 0));
        label_7->setText(QApplication::translate("BatchAddDialog", "\346\234\210", 0));
        label_8->setText(QApplication::translate("BatchAddDialog", "\346\227\245", 0));
        label_10->setText(QApplication::translate("BatchAddDialog", "\346\227\266\351\227\264\351\227\264\351\232\224\357\274\232", 0));
        y_button->setText(QApplication::translate("BatchAddDialog", "\345\271\264", 0));
        m_button->setText(QApplication::translate("BatchAddDialog", "\346\234\210", 0));
        w_button->setText(QApplication::translate("BatchAddDialog", "\345\221\250", 0));
        d_button->setText(QApplication::translate("BatchAddDialog", "\346\227\245", 0));
        pushButton->setText(QApplication::translate("BatchAddDialog", "\347\241\256\345\256\232", 0));
        pushButton_2->setText(QApplication::translate("BatchAddDialog", "取消", 0));
    }

};

#endif // BATCHADDDIALOG_H
