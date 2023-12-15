-- Создание таблицы для ассетов
CREATE TABLE IF NOT EXISTS Assets (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    path TEXT NOT NULL,
    type TEXT,
    size INTEGER,
    metadata TEXT
);

-- Добавление данных об ассетах
INSERT INTO Assets (name, path, type, size, metadata) VALUES 
    ('Background Image', '/assets/images/bg.jpg', 'image', 1024, 'Background image for the game'),
    ('Main Character', '/assets/characters/main_character.png', 'image', 512, 'Main character sprite'),
    ('Background Music', '/assets/audio/bg_music.mp3', 'audio', 2048, 'Background music track');
