CREATE TABLE teachers (
    teacher_id  INT PRIMARY KEY,                 -- 教师 ID（主键默认自增）
    name        VARCHAR(50) NOT NULL,            -- 姓名
    department  VARCHAR(100) NOT NULL            -- 所属学院
);
