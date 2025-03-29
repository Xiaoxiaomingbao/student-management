CREATE TABLE students (
    student_id   INTEGER PRIMARY KEY AUTOINCREMENT,  -- 学号
    name         TEXT NOT NULL,                      -- 姓名
    gender       SMALLINT NOT NULL,                  -- 性别（0 表示女，1 表示男）
    class_name   TEXT NOT NULL                       -- 班级
);
