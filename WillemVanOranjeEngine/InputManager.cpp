#include "InputManager.h"

Willem::InputManager::InputManager()
    :m_ControllerButtons{ ControllerButton::ButtonA,ControllerButton::ButtonB, ControllerButton::ButtonX, ControllerButton::ButtonY,ControllerButton::ButtonUp,ControllerButton::ButtonRight, ControllerButton::ButtonDown, ControllerButton::ButtonLeft,
   ControllerButton::TriggerLeft,ControllerButton::TriggerRight, ControllerButton::LeftThumbStickLeft, ControllerButton::LeftThumbStickRight, ControllerButton::LeftThumbStickUp, ControllerButton::LeftThumbStickDown,
    ControllerButton::RightThumbStickLeft,ControllerButton::RightThumbStickRight, ControllerButton::RightThumbStickUp, ControllerButton::RightThumbStickDown,ControllerButton::ButtonStart }
{
    for (size_t i = 0; i < XUSER_MAX_COUNT; i++)
    {
        m_Controllers.push_back(std::make_unique<Controller>());
    }

    m_KeyboardController = std::make_unique<KeyboardController>();

}

bool Willem::InputManager::ProcessInput()
{
    ControllerInput();
    return KeyboardInput();
}

void Willem::InputManager::ControllerInput()
{
    DWORD dwResult;
    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {

        ZeroMemory(&(m_Controllers[i]->m_CurrentState), sizeof(XINPUT_STATE));
        dwResult = XInputGetState(i, &(m_Controllers[i]->m_CurrentState));

        for (ControllerButton button : m_ControllerButtons)
        {
            ControllerKey key = std::make_pair(int(button), button);

            if (m_Controllers[i]->m_ConsoleCommands.find(key) == m_Controllers[i]->m_ConsoleCommands.end())
                continue;

            const auto& command = m_Controllers[i]->m_ConsoleCommands.at(key).first;
            const int& parameter = m_Controllers[i]->m_ConsoleCommands.at(key).second;
            if (!command->GetActivated())
            {

                if (IsPressed(button, m_Controllers[i]))
                {
                    command->Execute(parameter);
                    command->SetActivated(true);
                }

            }
            else
            {
                if (!IsPressed(button, m_Controllers[i]))
                {
                    command->Release(parameter);
                    command->SetActivated(false);
                }
            }

        }

    }
}

bool Willem::InputManager::KeyboardInput()
{
    SDL_Event ev;

    if (SDL_PollEvent(&ev))
    {
        if (ev.type == SDL_QUIT)
            return false;


        if (ev.type == SDL_KEYDOWN)
        {
            KeyboardKey key = std::make_pair(int(ev.key.keysym.sym), KeyboardButton(int(ev.key.keysym.sym)));
            KeyboardCommandsMap& commandMap = m_KeyboardController.get()->m_ConsoleCommands;

            if (commandMap.find(key) != commandMap.end())
            {
                const auto& command = commandMap.at(key).first.get();
                const int& parameter = commandMap.at(key).second;
                command->Execute(parameter);
                command->SetActivated(true);

            }

        }
        if (ev.type == SDL_KEYUP)
        {
            KeyboardKey key = std::make_pair(int(ev.key.keysym.sym), KeyboardButton(int(ev.key.keysym.sym)));
            KeyboardCommandsMap& commandMap = m_KeyboardController.get()->m_ConsoleCommands;

            if (commandMap.find(key) != commandMap.end())
            {
                const auto& command = commandMap.at(key).first.get();
                const int& parameter = commandMap.at(key).second;
                command->Release(parameter);
                command->SetActivated(false);

            }
        }
    }


    return true;
}


bool Willem::InputManager::IsPressed(const ControllerButton& button, const std::unique_ptr<Controller>& controller) const
{
    if (button == ControllerButton::TriggerLeft)
        return IsPressed(controller->m_CurrentState.Gamepad.bLeftTrigger);

    if (button == ControllerButton::TriggerRight)
        return IsPressed(controller->m_CurrentState.Gamepad.bRightTrigger);

    if (button == ControllerButton::LeftThumbStickLeft)
        return IsPressed(controller->m_CurrentState.Gamepad.sThumbLX,false);
                                                                         
    if (button == ControllerButton::LeftThumbStickRight)                 
        return IsPressed(controller->m_CurrentState.Gamepad.sThumbLX,true);
                                                                         
    if (button == ControllerButton::LeftThumbStickUp)                    
        return IsPressed(controller->m_CurrentState.Gamepad.sThumbLY, true);
                                                                        
   if (button == ControllerButton::LeftThumbStickDown)                  
       return IsPressed(controller->m_CurrentState.Gamepad.sThumbLY, false);
                                                                         
    if (button == ControllerButton::RightThumbStickRight)                
        return IsPressed(controller->m_CurrentState.Gamepad.sThumbRX, false);
                                                                        
    if (button == ControllerButton::RightThumbStickLeft)                
        return IsPressed(controller->m_CurrentState.Gamepad.sThumbRX, true);
                                                                        
    if (button == ControllerButton::RightThumbStickUp)                  
        return IsPressed(controller->m_CurrentState.Gamepad.sThumbRY, true);
                                                                        
    if (button == ControllerButton::RightThumbStickDown)                
        return IsPressed(controller->m_CurrentState.Gamepad.sThumbRY, false);

    WORD buttonState = controller->m_CurrentState.Gamepad.wButtons;
    
    if (buttonState & int(button))
        return true;

    return false;
}

bool Willem::InputManager::IsPressed(const BYTE& trigger) const
{
    if (float(trigger) / m_TriggerRange > m_TriggerSensitivity)
    {
        return true;
    }
    return false;
}

bool Willem::InputManager::IsPressed(const SHORT& analogMovement,bool up) const
{
    if (up)
    {
        if (fmaxf(0, (float)analogMovement / m_AnalogRange) > m_AnalogSensitivity)
        {
            return true;
        }
    }
    else
    {
        if (fmaxf(0, -(float)analogMovement / m_AnalogRange) > m_AnalogSensitivity)
        {
            return true;
        }
    }

    return false;
}