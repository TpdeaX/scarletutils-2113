# Scarlet Utils (Port a Geometry Dash 2.113)

Port completo de **Scarlet Utils** (originalmente un mod de Geode para GD 2.2) a **Geometry Dash 2.113 (Win32 x86)** como una DLL inyectable nativa.

Este port utiliza las direcciones de memoria oficiales de 2.113 contenidas en [`bindings-main/bindings/2.113/GeometryDash.bro`](../bindings-main/bindings/2.113/GeometryDash.bro), MinHook para los hooks en tiempo de ejecución, e ImGui sobre OpenGL para la interfaz gráfica.

---

## 📌 Tabla de Direcciones de Memoria (GD 2.113 Windows x86)

Todas las direcciones son relativas a la dirección base del juego (`gd_base = GetModuleHandleA(NULL)`):

| Clase | Función / Método | Offset 2.113 (Win32) | Propósito en Scarlet Utils |
|---|---|---|---|
| **GJBaseGameLayer** | `pushButton(int, bool)` | `0x111500` | Auto Swift, Extra Clicks, Autoclicker, Straight Fly/UFO |
| **GJBaseGameLayer** | `releaseButton(int, bool)` | `0x111660` | Auto Swift, liberación de clicks, Straight Fly/UFO |
| **GJBaseGameLayer** | `bumpPlayer(PlayerObject*, GameObject*)` | `0x10ED50` | Detección de Jump Pads (8, 9, 34) y Gravity Pads (10) |
| **PlayLayer** | `init(void*)` | `0x1FB780` | Registro del puntero global `g_currentPlayLayer` |
| **PlayLayer** | `~PlayLayer()` | `0x1FAFC0` | Limpieza del puntero de `PlayLayer` al salir del nivel |
| **PlayLayer** | `update(float)` | `0x2029C0` | Bucle principal: Straight Fly, Straight UFO, Autoclicker, orbes |
| **PlayLayer** | `addObject(GameObject*)` | `0x2017E0` | Layout Mode (ocultar decoraciones, remover glow), No Effects |
| **PlayLayer** | `destroyPlayer(PlayerObject*, GameObject*)` | `0x20A1A0` | Noclip (P1/P2), registro de muerte para Flip on Death |
| **PlayLayer** | `resetLevel()` | `0x20BF00` | Flip on Death (inversión de inputs, des-congelar unfreeze) |
| **PlayLayer** | `startGame()` | `0x1FD390` | Level Fade In, Restart First Frame |
| **PlayLayer** | `showEndLayer()` | `0x1FFD00` | Hide Endscreen, Level Fade Out, Audio Fade Out |
| **PlayLayer** | `showNewBest(...)` | `0x1FE3A0` | Hide New Best |
| **PlayLayer** | `playGravityEffect(bool)` | `0x207E00` | No Effects (anular efecto de gravedad) |
| **PlayLayer** | `onEnterTransitionDidFinish()` | `0x20DBD0` | Desactivar brillo/glitter en Layout Mode |
| **PlayLayer** | `toggleGlitter(bool)` | `0x20A0D0` | Desactivación de glitter |
| **PlayLayer** | `updateColor(...)` | `0x207070` | Colores personalizados de fondo y suelo en Layout Mode |
| **PlayerObject** | `spawnPortalCircle(ccColor3B, float)` | `0x1EF680` | No Effects (anular círculo de portal) |
| **PlayerObject** | `playDeathEffect()` | `0x1EFBE0` | No Death Effect |
| **PlayerObject** | `stopDashing()` | `0x1EE6E0` | Stacked Orb Optimization |
| **PlayerObject** | `propellPlayer(float)` | `0x1F8B50` | Click Gravity Pads |
| **GameObject** | `addGlow()` | `0xCFEF0` | Layout Mode (ocultar decoraciones adicionales) |
| **GameObject** | `playShineEffect()` | `0xEAB20` | No Effects (anular brillo de objetos) |
| **RingObject** | `spawnCircle()` | `0x2523A0` | Stacked Orb Optimization & No Effects |
| **CCCircleWave** | `draw()` | `0x17100` | No Effects (anular ondas circulares) |
| **PauseLayer** | `customSetup()` | `0x1E4620` | Botón para abrir Scarlet Utils en menú de pausa |
| **EndLevelLayer** | `customSetup()` | `0x94CB0` | Hide Endscreen |

---

## 🚀 Características Implementadas

### 🎮 Pestaña "Gameplay"
- **Flip Input On Death**: Invierte automáticamente la lógica de salto tras respawnear.
  - Opciones: Player 1, Player 2, Click Both, Swift, Unfreeze (envía tecla `V`).
- **Click Green Dash Orbs**: Salto instantáneo automático al tocar dash orbs verdes.
- **Click Black Orbs & Easy Black Orb UFO**: Salto automático al caer hacia orbes negras.
- **Click Jump Pads & Gravity Pads**: Detección de pads amarillos, rosas, rojos y pads azules de gravedad con auto swift click.
- **Maintain Gravity**: Sincronización continua del estado de salto al invertir la gravedad.
- **Auto Straight Fly**: Estabilización de nave manteniendo la velocidad vertical en 0 con umbrales configurables para P1 y P2.
- **Auto Straight UFO**: Estabilización automática del UFO en una coordenada Y específica para P1 y P2.
- **Autoclicker P1 & P2**: Intervalo de frames configurables entre clicks y duración del salto, con modo normal o swift.
- **Noclip**: Inmunidad total a colisiones letales configurable por separado para P1 y P2.
- **Auto Swift**: Libera automáticamente el click al presionar.
- **Extra Clicks**: Multiplicador de clicks configurable.

### 🎨 Pestaña "Visual"
- **Level Fade In/Out**: Fundido a negro configurable al iniciar y completar el nivel.
- **Audio Fade In/Out**: Fundido de volumen musical configurable.
- **Hide Endscreen**: Oculta la pantalla de estadísticas final tras completar un nivel.
- **Hide New Best**: Oculta el mensaje emergente de nuevo récord.
- **No Death Effect**: Desactiva la explosión/animación de muerte del jugador.
- **No Effects**: Elimina ondas, partículas, portales y brillos para máxima claridad y FPS.
- **Layout Mode**: Transforma cualquier nivel a modo estructura (layout) con colores personalizables de fondo y suelo, filtrando decoraciones y ocultando el brillo de objetos.
- **Restart First Frame**: Reinicia automáticamente el nivel en el primer frame para bots y renderers.
- **Stacked Orb Optimization**: Evita duplicación de efectos si se presionan múltiples orbes en el mismo tick.

---

## 🛠️ Cómo Compilar

### Requisitos:
1. **CMake** 3.20 o superior.
2. **Visual Studio 2019 / 2022** con el paquete *"Desarrollo para el escritorio con C++"*.
3. **Arquitectura Win32 (x86)**: Geometry Dash 2.113 es un ejecutable de 32 bits.

### Pasos de compilación:
Abre una terminal (PowerShell o CMD) en esta carpeta:

```bash
# 1. Configurar proyecto en 32-bits (Win32)
cmake -B build -A Win32

# 2. Compilar en modo Release
cmake --build build --config Release
```

El archivo compilado se generará en:
`build/Release/scarletutils_2113.dll`

---

## 💉 Cómo Inyectar en GD 2.113

1. Abre **Geometry Dash 2.113**.
2. Usa cualquier inyector DLL de 32 bits (por ejemplo: **Cheat Engine**, **Xenos Injector**, **Process Hacker**, o un cargador de DLL como QuickLdr / MinHook injector).
3. Selecciona el proceso `GeometryDash.exe`.
4. Inyecta `scarletutils_2113.dll`.
5. Presiona la tecla **`F`** (o **`TAB`**) dentro del juego para abrir el menú de Scarlet Utils.
6. La configuración se guarda automáticamente en `scarletutils_2113.ini` en la carpeta del juego.
