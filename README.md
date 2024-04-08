# MQ2GMChatCC

MQ2GMChatCC is a MacroQuest plugin designed to handle responses to Game Master (GM) greetings in EverQuest and Camp Check in /ooc from players.

## Features

- Automatically responds to GM greetings with customizable phrases.
- Automatically responds to Camp Check with customizable phrases.
- Supports both `/say` and `/tell` responses.
- Cooldown period to prevent excessive responses.
- Easy configuration through an INI file.

## Installation

1. Download the latest release of MQ2GMChatCC from the [releases page](https://github.com/spik3n/MQ2GMChatCC/releases).
2. Extract the .dll file into your MacroQuest `plugins` folder and the .ini into your MacroQuest `config` folder.

## Usage

1. Make sure MacroQuest 2 is installed and running.
2. Ensure that `MQ2GMChatCC` is loaded by typing `/plugin MQ2GMChatCC` in the EverQuest chat window.
3. Create a configuration file named `MQ2GMChatCC.ini` with your desired settings. See the section below for details on the INI file format.
4. Place the `MQ2GMChatCC.ini` file in the `config` folder of your EverQuest installation directory.
5. Once configured, the plugin will automatically respond to GM greetings based on the settings in the INI file.

## INI File

The INI file `MQ2GMChatCC.ini` should be structured as follows:

```ini
[Settings]
CooldownSeconds=3600
; You can use either /ooc CHAT MESSAGE or you can use /keypress KEY
; Example 1: /keypress 1
; Example 2: /ooc Camp X taken
CampCheckResponse=/ooc Camp X taken
; This is the reaction order what to do on GM message, right now first it will replay, then do /end for macro and then /unload. But you can pick what you want
Reaction1=/end
Reaction2=/unload

[Phrases]
1=Good day! What can I do for you?
2=Howdy! What brings you by today?
3=How may I be of service to you?
4=Pardon me, I'm currently occupied. Can I assist you later on?
5=Of course! What assistance do you require?
6=Salutations! What brings you to us today?
7=Welcome! How may I assist you today?
8=I'm here to assist! What's on your agenda?
9=Hey! How can I lend a hand today?
10=Greetings and salutations! How may I be of assistance?
11=Welcome! Is there something specific you need help with?
12=Hey there! What's on your mind?
13=Good to see you! Feel free to ask for any assistance.
14=Aloha! If you need assistance, just let me know.
15=Howdy! What can I help you with today?
16=Bonjour! How may I be of service to you?
17=Namaste! What can I assist you with today?
18=Hola! What brings you here today?
19=Hey! Do you need assistance with anything?
20=Welcome! How may I assist you today?
21=G'day! Need help with anything specific?
22=Konnichiwa! How can I assist you today?
23=Howdy! What brings you here today?
24=Salutations! What's on your mind?
25=Good day! How can I assist you?
26=Greetings and salutations! Need assistance with anything?
27=Hey there! What do you need help with today?
28=Good to see you! What brings you here today?
29=Bonjour! Is there anything I can assist you with?
30=Namaste! How can I help you today?
31=Hola! Need assistance with something?
32=Konnichiwa! What do you need help with?
33=G'day! How may I assist you?
34=Good day! Need any assistance today?
35=Salutations! Let me know if you need help.
36=Hey there! How can I assist you today?
37=Welcome! Is there anything I can help you with?
38=Greetings! Need assistance with anything specific?
39=Howdy! What do you need assistance with?
40=Good to see you! How can I assist today?
41=Aloha! Need any help from me?
42=Namaste! What assistance do you require?
43=Hey there! What's on your mind?
44=G'day! How may I be of service?
45=Salutations! Is there anything I can assist with?
46=Konnichiwa! What assistance do you require?
47=Hola! Need any help from me?
48=Good day! How may I assist you?
49=Howdy! Is there anything I can assist with?
50=Welcome! What assistance do you require today?
