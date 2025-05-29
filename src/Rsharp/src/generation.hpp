#pragma once

class Generator{
public:
    inline Generator(N_Exit root)
        : m_root(move(root)){
        }

        [[nodiscard]] string generate() const{
            stringstream out; 
            out << "global _start\n_start:\n";
            out << "    mov rax, 60\n"; 
            out << "    mov rdi, " << m_root.expr.integer.value.value() << endl;
            out << "    syscall";
            return out.str();
        }

private:
    const N_Exit m_root;
};