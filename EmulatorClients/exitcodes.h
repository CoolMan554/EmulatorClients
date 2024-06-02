#ifndef EXITCODES_H
#define EXITCODES_H

/**
 * @brief The ExitCodes class
 * Класс определяет перечисление ExitCode, которое содержит возможные коды завершения программы
 */
class ExitCodes
{
public:
    enum ExitCode : int
    {
        Error = -1,
        Exit,
        Restart
    };
};

#endif // EXITCODES_H
