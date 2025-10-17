/**
 * @file messagebox.cpp
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @brief handles the message box for AP messages
 * @date 2025-08-09
 * 
 */

#include "hintmessagebox.h"
#include "LB1AP.h"




HintMessageBox::HintMessageBox(DWORD BASE_ADDR):
    BASE_ADDR(BASE_ADDR),
    timer(*((volatile float*)(BASE_ADDR + 0x006C5F34))),
    shownHint(*((volatile DWORD*)(BASE_ADDR + 0x006C5F24))),
    theme(*((volatile BYTE*)(BASE_ADDR + 0x006C5F29))),
    text(
        reinterpret_cast<char*>(
            reinterpret_cast<uintptr_t>(
                *reinterpret_cast<char**>(BASE_ADDR + 0x006947F8)
            ) + 0x2039
        )
    ),

    messageSpacing(1.0),
    hold(false)
{
    std::cout << "initializing timer" << std::endl;
    timer = 0;
    timerPrev = timer;
    std::cout << "initialized hintmessage box" << std::endl;
}

HintMessageBox::~HintMessageBox()
{

}

void HintMessageBox::setText(std::string text)
{
    constexpr size_t limit = 324; // buffer size including '\0'

    if (text.size() >= limit) { // WARN: LIMIT untested
        // TODO: test
        size_t copyLen = limit - 4; // space for "..." + '\0'
        memcpy(this->text, text.c_str(), copyLen);
        this->text[copyLen]     = '.';
        this->text[copyLen + 1] = '.';
        this->text[copyLen + 2] = '.';
        this->text[copyLen + 3] = '\0';
    }
    else {
        // copy string + terminator
        memcpy(this->text, text.c_str(), text.size() + 1);
    }

    timer = 0.0; // TODO: Hope this works...
}

void HintMessageBox::tick() {
    // check if things have been interrupted
    if (shownHint != hintId) {
        // intial or hint was cleared.
        shownHint = hintId;
        std::cout << "id doesn't match prev " << std::hex << shownHint << " " << hintId << std::endl;
    }

    // check if game as force set the timer.
    // TODO: this needs to be improved eventually.
    if (timer != timerPrev) {
        std::cout << "timer doesn't match prev " << (float) timer << " " << timerPrev << std::endl;
        timer = -2.0; // buffer
    }

    if (hold) return; // holding message


    // subtract timer.
    // check if hint has expired
    if (timer >= 7.5 + messageSpacing) {
        // next hint.
        AP_Message* message = LB1AP_GetMessage();
        if (message != nullptr) {
            std::cout << "next hint" << std::endl;
            // there is a new message
            switch (message->type)
            {
            case AP_MessageType::Plaintext: { // TODO: does this include chat? if so might want to put this somewhere else.
                // theme = 4;
                //setText("Plaintext: " + message->text);
                return;
            }
            case AP_MessageType::ItemSend: {
                // theme = (theme + 1) % 4;
                AP_ItemSendMessage* msg = static_cast<AP_ItemSendMessage*>(message);
                setText(msg->item + " was sent to " + msg->recvPlayer);
                break;
            }
            case AP_MessageType::ItemRecv: {
                // theme = (theme + 1) % 4;
                AP_ItemRecvMessage* msg = static_cast<AP_ItemRecvMessage*>(message);
                setText( msg->sendPlayer + " sent you " + msg->item);
                break;
            }
            case AP_MessageType::Hint: {
                // theme = 4;
                AP_HintMessage* msg = static_cast<AP_HintMessage*>(message);
                setText("Hint: " + msg->text);
                break;
            }
            case AP_MessageType::Countdown: { // TODO: add this functionality. It would need to use hold and settext instead of waiting out the timer each time.
                // theme = 5;
                // WARN: this is untested
                tick(); // skip
                return;
                break;
            }

            default:
                // Unkown message type
                std::cerr << "Attempted Unknown message type in hint message box." << std::endl;
                return;
            }

            //timer = 0.0; // TODO: May want to adjust this closer to 7 for faster messages.
        }
    } else {
        timer += 0.01;
        timerPrev = timer;
    }
}

void HintMessageBox::holdMessage()
{
    hold = true;
}

void HintMessageBox::holdMessage(std::string text)
{
    if (text.empty()) { // TODO: is this correct?
        std::cerr << "holdMessage text was empty." << std::endl;
        return;
    }
    // may want to also store/restore previous message if used for other things.
    shownHint = hintId;
    setText(text);
    std::cout << "holdMessage clearing timer" << std::endl;
    timer = 0;
    timerPrev = timer;
    hold = true;
}

void HintMessageBox::releaseMessage()
{
    hold = false;
}
