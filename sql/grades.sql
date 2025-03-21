CREATE TABLE grades (
    grade_id    INT PRIMARY KEY,                 -- 成绩 ID（主键默认自增）
    student_id  INT NOT NULL,                    -- 学生 ID（外键）
    course_id   INT NOT NULL,                    -- 课程 ID（外键）
    score       DECIMAL(5,2) NOT NULL,           -- 分数
    FOREIGN KEY (student_id) REFERENCES students(student_id),
    FOREIGN KEY (course_id) REFERENCES courses(course_id)
);
