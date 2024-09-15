#include "custemitem.h"

custemItem::custemItem(QListWidgetItem* pItem, QWidget *parent) :
    QWidget(parent),m_pItem(pItem)
{

    // 创建布局
    m_pLayout = new QVBoxLayout(this);
    m_pLayout->setContentsMargins(0, 0, 0, 0); // 设置内容边距为0

    // 创建图片标签
    m_pImageLabel = new QLabel(this);
    m_pImageLabel->setAlignment(Qt::AlignCenter); // 图片居中显示
    m_pImageLabel->setFixedSize(100, 100); // 设置图片大小，可以根据需要调整

    // 创建标题标签
    m_pLabel = new QLabel(this);
    m_pLabel->setAlignment(Qt::AlignCenter); // 标题居中显示

    // 将图片和标题添加到布局中
    m_pLayout->addWidget(m_pImageLabel);
    m_pLayout->addWidget(m_pLabel);

    // 设置双击事件
    connect(this, &custemItem::doubleClicked, this, &custemItem::on_doubleClicked);

    // 设置项数据
    if (m_pItem) {
        m_pLabel->setText(m_pItem->text());
        // 假设图片路径存储在QListWidgetItem的data中
        modelPath = m_pItem->data(Qt::UserRole).toString();
        if (!modelPath.isEmpty()) {
            QPixmap pixmap(modelPath);
            m_pImageLabel->setPixmap(pixmap.scaled(m_pImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}


void custemItem::on_doubleClicked()
{
    emit selectModel(m_pItem);
}

