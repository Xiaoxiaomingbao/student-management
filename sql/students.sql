CREATE TABLE students (
    student_id   INT PRIMARY KEY,                 -- 学号（主键默认自增）
    name         VARCHAR(50) NOT NULL,            -- 姓名
    gender       SMALLINT NOT NULL,               -- 性别（0 表示女，1 表示男）
    class_name   VARCHAR(50) NOT NULL             -- 班级
);
