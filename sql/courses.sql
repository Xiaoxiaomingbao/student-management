CREATE TABLE courses (
    course_id   INTEGER PRIMARY KEY AUTOINCREMENT,    -- 课程 ID
    name        TEXT NOT NULL,                        -- 课程名称
    credit      INTEGER NOT NULL,                     -- 学分
    teacher_id  INTEGER NOT NULL,                     -- 讲师 ID
    FOREIGN KEY (teacher_id) REFERENCES teachers(teacher_id)
);
