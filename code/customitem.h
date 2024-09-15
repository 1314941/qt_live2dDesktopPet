#ifndef CUSTEMITEM_H
#define CUSTEMITEM_H

#include <QWidget>
#include <QListWidgetItem>

/*
自定义的列表项控件
未使用
*/


class custemItem : public QWidget
{
    Q_OBJECT

public:
    custemItem(QListWidgetItem* pItem, QWidget *parent = nullptr);

signals:
    void selectedModel(QListWidgetItem* pItem);

private slots:
    void on_doubleClicked();

private:
    QLabel* m_pLabel; // 用于显示标题
    QLabel* m_pImageLabel; // 用于显示图片
    QVBoxLayout* m_pLayout; // 用于布局

    QListWidgetItem* m_pItem;
    QString modelPath;
};

#endif // CUSTEMITEM_H

