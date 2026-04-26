#ifndef BAR_H
#define BAR_H

#include <QObject>
#include <QColor>


class Bar : public QObject
{
    Q_OBJECT
        // tao có biến x, nếu đọc thì dùng x() , còn nếu viết vào thì dùng setX(), tao sẽ báo changed để nhận dạng
        Q_PROPERTY(int x READ x WRITE setX NOTIFY changed)

public:
    //hàm khởi tạo với giá trị thanh, tọa độ ngang x, và dọc y(thanh này quyết định Ui thanh bar sẽ bắt đầu vẽ từ đâu đến điểm kết thúc ở đâu)
    Bar(int value, int x, int baseY, QObject* parent = nullptr)
        : QObject(parent), value(value), m_x(x), m_baseY(baseY) 
    {
        // màu sắc thanh bar mặc định
        color = QColor(100, 150, 255); // tím nhạt 
    }

    //hàm x để đưa ra x
    int x() const { return m_x; }

    // hàm set 
    void setX(int x)
    {
        // nếu giá trị đã tương tự trả về luôn
        if (m_x == x) return;
        m_x = x; // nếu khác thì gán
        //gửi t
        emit changed();
    }

    // giá trị của thanh bar cũng như đối tượng màu sắc
    int value;
    QColor color;

    // tín hiệu báo hiệu thay đổi đối với đối tượng bar, phát ra để làm tín hiệu nhận biết xử lý các slot liên quan
signals:
    void changed();

    // khởi tạo x là tọa độ của các thanh biểu diễn, y là đích của paint 
private:
    int m_x;
    int m_baseY;
};

#endif