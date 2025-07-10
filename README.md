
![Security](https://img.shields.io/badge/Security-Training-red)
![Language](https://img.shields.io/badge/Language-C-blue)
![Platform](https://img.shields.io/badge/Platform-Windows-green)
![License](https://img.shields.io/badge/License-MIT-yellow)
![Educational](https://img.shields.io/badge/Purpose-Educational-orange)

# 🎭 BloodMary Phishing Simulator

## 📋 Description

**BloodMary** is an educational tool (trojan-virus) for training cybersecurity specialists, simulating realistic phishing attacks with social engineering elements. This project was created to raise awareness about cyber threats and demonstrate the consequences of running suspicious files.

> ⚠️ **WARNING**: This tool is intended EXCLUSIVELY for educational purposes and authorized testing in controlled environments.

---

## 🎯 Project Goals

- **Training** personnel in cybersecurity fundamentals
- **Demonstrating** realistic phishing techniques
- **Raising awareness** about social engineering
- **Testing** readiness for cyber threats

---

## 🔧 Technical Capabilities

### **🕵️ Reconnaissance Techniques**
- ✅ System information gathering (OS, processor, RAM)
- ✅ Network configuration analysis (IP, MAC, adapters)
- ✅ Active process enumeration
- ✅ Username and computer name identification

### **🛡️ Anti-Sandbox**
- ✅ Execution time verification (bypass time acceleration)
- ✅ Process count analysis (sandbox detection)
- ✅ RAM volume checking (VM detection)
- ✅ Activation delay (behavioral analysis evasion)

### **📡 Data Exfiltration**
- ✅ TCP transmission (SSH imitation)
- ✅ HTTP fallback for reliability
- ✅ Network settings obfuscation
- ✅ Unique filename generation

### **🎭 Social Engineering**
- ✅ Realistic visual effects
- ✅ Psychological impact
- ✅ Ransomware behavior imitation
- ✅ Educational messages

---

## 🎬 Gaming Elements

### **🩸 Visual Effects**
- **Realistic Blood**: Physical simulation of drops with gravity
- **Ghostly Messages**: Flickering text with glow effect
- **Fullscreen Animation**: Optimized for Full HD (1920x1080)
- **Double Buffering**: Smooth animation without flickering

### **🎨 Effect Technical Details**
```c
// Blood drop physics
drops[i].velocityY += drops[i].acceleration;
drops[i].x += drops[i].velocityX;
drops[i].y += drops[i].velocityY;

// Color gradient creation
COLORREF CreateBloodGradient(int baseRed, int variation, int alpha)
```

### **🎪 Impact Scenario**
1. **Stealth Launch** - No visible signs
2. **Data Collection** - Silent system analysis
3. **Anti-Sandbox** - Virtual environment check
4. **Exfiltration** - Data transmission to "C&C server"
5. **Psychological Effect** - Dramatic visualization
6. **Educational Finale** - Explanation of what happened

---

## 🛠️ Build and Installation

### **Requirements**
- **Arch Linux** (or any Linux with MinGW-w64)
- **MinGW-w64** cross-compiler
- **UPX** for compression (optional)
- **Wine** for testing (optional)

### **Quick Installation**
```bash
# Install dependencies
sudo pacman -S mingw-w64-gcc mingw-w64-binutils mingw-w64-headers mingw-w64-crt upx wine

# Clone repository
git clone https://github.com/toxy4ny/bl00dym4ry.git
cd bl00dym4ry

# Build
make
# or
./build.sh

# Build both versions (32-bit and 64-bit)
make all
# or
./build.sh all
```

### **Manual Build**
```bash
# 64-bit version
x86_64-w64-mingw32-gcc -Os -s -static -DWIN32_LEAN_AND_MEAN \
    -ffunction-sections -fdata-sections -fno-ident -fomit-frame-pointer \
    -o bl00dym3ry.exe main.c \
    -Wl,--gc-sections -Wl,--strip-all -Wl,--build-id=none \
    -static-libgcc -static-libstdc++ \
    -lwininet -lws2_32 -liphlpapi -luser32 -lkernel32 -lgdi32 \
    -lshell32 -ladvapi32 -lole32 -loleaut32 -luuid -lmsimg32

# Size optimization
x86_64-w64-mingw32-strip --strip-all bl00dym3ry.exe
upx --best --lzma bl00dym3ry.exe
```

---

## 🎓 Educational Scenarios

### **📧 Phishing Campaigns**
1. **Email Attachment**: Disguised as document or image
2. **USB Drop**: Placed on USB drive with attractive name
3. **Social Media**: Distributed as "interesting file"
4. **Corporate Network**: Test employee awareness

### **🎯 Target Groups**
- **IT Specialists** - Technical threat understanding
- **Managers** - Business risk comprehension
- **Regular Users** - Cybersecurity hygiene basics
- **Students** - Practical learning

---

## 📊 Reporting

### **Automatic Reports**
- **Desktop Report**: `SECURITY_TRAINING_REPORT.txt` on desktop
- **Remote Logging**: Server transmission for analysis
- **Timestamp**: Precise execution time
- **System Fingerprint**: Unique system identification

### **Report Structure**
```
===============================================================
                    CYBERSECURITY TRAINING REPORT
===============================================================

WARNING: This is the result of a phishing training test

YOU SUCCESSFULLY LAUNCHED A SUSPICIOUS FILE!

What happened:
+ System information was collected
+ Data was sent to external server
+ Visual effects were demonstrated
+ Malicious activity was simulated

RECOMMENDATIONS:
1. Don't open suspicious attachments
2. Verify email senders
3. Use antivirus software
4. Regularly update software
5. Be careful with links
```

---

## 🔒 Security and Ethics

### **✅ Legal Usage**
- Authorized testing in own infrastructure
- Educational programs with participant consent
- Threat demonstration in controlled environment
- Corporate cybersecurity training

### **❌ Prohibited Usage**
- Attacks on foreign systems without permission
- Causing real harm or damage
- Violating computer crime legislation
- Commercial use without license

### **🛡️ Safety Measures**
```c
// File does NOT cause real harm:
// - Does not encrypt files
// - Does not install backdoors
// - Does not modify system settings
// - Creates only educational report
```

---

## 🎥 Operation Demonstration

### **Phase 1: Stealth Launch** 
```
[✓] Anti-sandbox checks passed
[✓] Console window hidden
[✓] Process started successfully
```

### **Phase 2: Reconnaissance**
```
[✓] System information collected
[✓] Network configuration analyzed  
[✓] Process enumeration completed
[✓] User credentials identified
```

### **Phase 3: Exfiltration**
```
[✓] Data packaged for transmission
[✓] TCP connection established
[✓] Information sent to C&C server
[✓] Connection terminated
```

### **Phase 4: Psychological Impact**
```
[✓] Screen effects activated
[✓] Horror elements displayed
[✓] Victim awareness achieved
[✓] Educational message delivered
```

---

## 🔧 Configuration and Customization

### **Network Settings**
```c
// Change obfuscated SSH settings
static char ssh_host[] = {0x73, 0x7f, 0x7f, 0x3a, 0x66, 0x6d, 0x63, 0x3a, 0x66, 0x3a, 0x66, 0x72, 0x72, 0x00};
static char ssh_user[] = {0x05, 0x16, 0x17, 0x03, 0x16, 0x02, 0x03, 0x00};
static char ssh_pass[] = {0x08, 0x34, 0x09, 0x16, 0x01, 0x02, 0x16, 0x08, 0x67, 0x65, 0x67, 0x69, 0x00};
```

### **Visual Effects**
```c
// Blood intensity configuration
#define BLOOD_DROPS_COUNT 80
#define BLOOD_SPLATTERS_COUNT 150
#define BLOOD_TRAILS_COUNT 300
#define ANIMATION_FRAMES 500
```

### **Anti-Sandbox**
```c
// Check configuration
#define MIN_SLEEP_TIME 2800
#define MIN_PROCESS_COUNT 25
#define MIN_RAM_SIZE (1024 * 1024 * 1024)
```

---

## 📚 Educational Materials

### **🎓 For Instructors**
- Use as practical assignment
- Explain each technique after demonstration
- Discuss protection methods
- Conduct debriefing with participants

### **🔬 For Researchers**
- Analyze evasion techniques
- Study obfuscation methods
- Develop countermeasures
- Improve detection systems

### **👥 For HR and Management**
- Demonstrate real risks
- Justify security investments
- Raise personnel awareness
- Create security culture

---

## 🤝 Project Participation

### **🐛 Bug Reports**
If you found a bug or issue:
1. Check existing issues
2. Create new issue with detailed description
3. Attach system information
4. Specify reproduction steps

### **💡 Improvement Suggestions**
- New evasion techniques
- Additional visual effects
- Information gathering improvements
- Performance optimization

### **🛠️ Pull Requests**
1. Fork repository
2. Create feature branch
3. Make changes
4. Write tests
5. Create pull request

---

## 📄 License

This project is distributed under the MIT License. See `LICENSE` file for details.

```
MIT License

Copyright (c) 2025 Hackteam.Red

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## ⚖️ Disclaimer

**IMPORTANT**: This project is created exclusively for educational purposes and authorized security testing. The authors are not responsible for improper use of this software.

Using this tool to attack systems without explicit permission from their owners is illegal and may result in criminal liability.

---

## 🔗 Useful Links

- **[OWASP](https://owasp.org/)** - Web security fundamentals
- **[NIST Cybersecurity Framework](https://www.nist.gov/cyberframework)** - Cybersecurity standards
- **[MITRE ATT&CK](https://attack.mitre.org/)** - Attack techniques database
- **[Social Engineering Toolkit](https://github.com/trustedsec/social-engineer-toolkit)** - Social engineering tools

---

## 🏆 Acknowledgments

Thanks to all project contributors and the cybersecurity community for their contribution to creating a safer digital world!

```
   ██████╗ ██╗      ██████╗  ██████╗ ██████╗ ███╗   ███╗ █████╗ ██████╗ ██╗   ██╗
   ██╔══██╗██║     ██╔═══██╗██╔═══██╗██╔══██╗████╗ ████║██╔══██╗██╔══██╗╚██╗ ██╔╝
   ██████╔╝██║     ██║   ██║██║   ██║██║  ██║██╔████╔██║███████║██████╔╝ ╚████╔╝ 
   ██╔══██╗██║     ██║   ██║██║   ██║██║  ██║██║╚██╔╝██║██╔══██║██╔══██╗  ╚██╔╝  
   ██████╔╝███████╗╚██████╔╝╚██████╔╝██████╔╝██║ ╚═╝ ██║██║  ██║██║  ██║   ██║   
   ╚═════╝ ╚══════╝ ╚═════╝  ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   
                                                                                    
                        🎭 Phishing Simulator for Cybersecurity Training 🎭
```

---

*Version: 1.0.0 | Last Updated: 2025*
