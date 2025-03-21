CREATE TABLE courses (
    course_id   INT PRIMARY KEY,                 -- 课程 ID（主键默认自增）
    name        VARCHAR(100) NOT NULL,           -- 课程名称
    credit      INT NOT NULL,                    -- 学分
    teacher_id  INT,                             -- 讲师 ID（外键）
    FOREIGN KEY (teacher_id) REFERENCES teachers(teacher_id)
);
