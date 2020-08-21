#pragma once

#include "Common.hpp"
#include "Utility/Allocator/MyAllocator.hpp"
#include "Utility/Point.hpp"
#include <vector>
#include <array>

namespace bnsGame {
    class Game : public MyScene::Scene {
    private:
        enum class MinoType {
            I,
            O,
            S,
            Z,
            J,
            L,
            T,
            End
        };

        enum MinoAngle {
            deg0,
            deg90,
            deg180,
            deg270,
            degEnd
        };

        static constexpr std::array<std::array<std::array<std::array<uint32_t, 4>, 4>, MinoAngle::degEnd>, static_cast<uint32_t>(MinoType::End)> k_minoShapes = {{
            // I
            {{
                // deg0
                {{
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0}
                }},
                // deg90
                {{
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0}        
                }},
                // deg180
                {{
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0}    
                }},
                // deg270
                {{
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}     
                }}
            }},
            // O
            {{
                // deg0
                {{
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0}
                }},
                // deg90
                {{
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0}        
                }},
                // deg180
                {{
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0}    
                }},
                // deg270
                {{
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}     
                }}
            }},
            // S
            {{
                // deg0
                {{
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0}
                }},
                // deg90
                {{
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0}        
                }},
                // deg180
                {{
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0}    
                }},
                // deg270
                {{
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}     
                }}
            }},
            // Z
            {{
                // deg0
                {{
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0}
                }},
                // deg90
                {{
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0}        
                }},
                // deg180
                {{
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0}    
                }},
                // deg270
                {{
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}     
                }}
            }},
            // J
            {{
                // deg0
                {{
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0}
                }},
                // deg90
                {{
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0}        
                }},
                // deg180
                {{
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0}    
                }},
                // deg270
                {{
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}     
                }}
            }},
            // L
            {{
                // deg0
                {{
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0}
                }},
                // deg90
                {{
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0}        
                }},
                // deg180
                {{
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0}    
                }},
                // deg270
                {{
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}     
                }}
            }},
            // T
            {{
                // deg0
                {{
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 0, 0}
                }},
                // deg90
                {{
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0}        
                }},
                // deg180
                {{
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0}    
                }},
                // deg270
                {{
                    {0, 0, 0, 0},
                    {1, 1, 1, 1},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}     
                }}
            }}
        }};

        static constexpr uint32_t k_fieldHeight = 22;
        static constexpr uint32_t k_fieldWidth = 12;

        std::vector<uint32_t*> m_displayBuffer;
        std::vector<uint32_t*> m_field;

        utl::Point m_playerPos;

        MinoType m_minoType;
        MinoAngle m_minoAngle;

        void ClearDisplay() const {
            system("cls");
        }
    public:
        Game(const InitData& init_);

        void Update() override;

        void UpdateChangeOut() override;

        void Draw() const override;
    };
}  // namespace bnsGame