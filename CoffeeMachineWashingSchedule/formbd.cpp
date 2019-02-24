#include "formbd.h"


FormBd::FormBd(QDialog *parent) :QDialog(parent)
{
    name = new QLabel(trUtf8("Имя"));
    surname = new QLabel(trUtf8("Фамилия"));
    group = new QLabel(trUtf8("Группа"));
    email = new QLabel(trUtf8("e-mail"));
    phone = new QLabel(trUtf8("Моб. телефон"));

    boxgroup = new QComboBox;
    QStringList lst;
    lst << "BD-11" << "BD-21" << "BD-31" << "BD-41";
    boxgroup->addItems(lst);

    buttonok = new QPushButton(trUtf8("Ok"));
    linename = new QLineEdit;
    linesurname = new QLineEdit;
    lineemail = new QLineEdit;
    linephone = new QLineEdit;

    QVBoxLayout *main = new QVBoxLayout;
    main->addWidget(name);
    main->addWidget(linename);
    main->addWidget(surname);
    main->addWidget(linesurname);
    main->addWidget(group);
    main->addWidget(boxgroup);
    main->addWidget(email);
    main->addWidget(lineemail);
    main->addWidget(phone);
    main->addWidget(linephone);
    main->addWidget(buttonok);

    setLayout(main);

    /* Метода для инициализации модели,
         * из которой будут транслироваться данные
         * */
        setupModel();

        /* Если строка не задана, то есть равна -1,
             * тогда диалог работает по принципу создания новой записи.
             * А именно, в модель вставляется новая строка и работа ведётся с ней.
             * */

                model->insertRow(model->rowCount(QModelIndex()));
                mapper->toLast();
}

/* Метод настройки модели данных и mapper
 * */
void FormBd::setupModel()
{
    /* Инициализируем модель и делаем выборку из неё
     * */
    model = new QSqlTableModel(this);
    model->setTable("students");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    /* Инициализируем mapper и привязываем
     * поля данных к объектам LineEdit
     * */
    mapper = new QDataWidgetMapper();
    mapper->setModel(model);
    mapper->addMapping(linesurname, 1);
    mapper->addMapping(linename, 2);
    mapper->addMapping(linephone, 3);
    mapper->addMapping(lineemail, 4);
    //mapper->addMapping(ui->MACLineEdit, 3);
    /* Ручное подтверждение изменения данных
     * через mapper
     * */
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    /* Подключаем коннекты от кнопок пролистывания
     * к прилистыванию модели данных в mapper
     * */
    //connect(ui->previousButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    //connect(ui->nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    /* При изменении индекса в mapper изменяем состояние кнопок
     * */
    //connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(buttonok(int)));
    connect(buttonok,SIGNAL(clicked()),this,SLOT(on_buttonok_accepted()));
}

void FormBd::on_buttonok_accepted()
{

    /* В противном случае производится вставка новых данных в таблицу
     * и диалог завершается с передачей сигнала для обновления
     * таблицы в главном окне
     * */

        mapper->submit();
        model->submitAll();
        emit signalReady();
        this->close();

}

void FormBd::accept()
{

}
