# Summary

Name: StreamHash5

Digest size: 512 bits

Performance: ~0.55 cpb on Intel Skylake

# StreamHash Family

The architecture of traditional hash functions, also known as [Merkle–Damgård
construction](https://en.wikipedia.org/wiki/Merkle%E2%80%93Damg%C3%A5rd_construction),
is based on iterative processing their state block and successive input blocks
with a one-way compression function.  The StreamHash family of hash functions
does not use a compression function.  Instead of a single state block, they use
a vector of independently processed state values.  Each input block is mixed
with each state vector value, and then transformed with an nonlinear functions.
A different non-linear function is used to transform each state vector value.

Security of the Merkle–Damgård construction is based on the attacker being
unable to control the output of the compression function.  Dissimilarly,
security of the StreamHash family is based the attacker being able to only
control one value of the state vector at a time.  Replacement of the complex,
multi-round compression function with a simple, invertible, non-linear
transformation results in material performance improvement, while maintaining
the required security properties.

# StreamHash5 Algorithm

StreamHash5 state vector consists of 4 elements.  The state vector is
transformed with non-linear functions based on the well-studied AES round.

    F1ₙ(x) = aesenc(x, constₙ)

    F2ₙ(x) = aesenc(aesenc(x, constₙ), constₙ)

Two AES rounds ensure full bit diffusion.

The initial digits of the π number are used as the const₁…const₄ values.

## Init and Update

<!-- ![StreamHash5 Computation Flow](/docs/streamhash5_flow.png) -->

           const₁  const₂  const₃  const₄
           (16 B)  (16 B)  (16 B)  (16 B)
              │       │       │       │
    input₁━━━┳┿━━━━━━┳┿━━━━━━┳┿━━━━━━┓│┏━━━128
    (16 B)   ▾▾      ▾▾      ▾▾      ▾▾▾
            [XOR]   [XOR]   [XOR]   [XOR]
              │       │       │       │
              ▾       ▾       ▾       ▾
            [F2₁]   [F2₂]   [F2₃]   [F2₄]
              │       │       │       │
    input₂━━━┳┿━━━━━━┳┿━━━━━━┳┿━━━━━━┓│┏━━━256
    (16 B)   ▾▾      ▾▾      ▾▾      ▾▾▾
            [XOR]   [XOR]   [XOR]   [XOR]
              │       │       │       │
              ▾       ▾       ▾       ▾
            [F2₁]   [F2₂]   [F2₃]   [F2₄]
              │       │       │       │
              ┊       ┊       ┊       ┊

## Final

              ┊       ┊       ┊       ┊
              │       │       │       │
    inputₙ━━━┳┿━━━━━━┳┿━━━━━━┳┿━━━━━━┓│┏━━━total input bits
    (<=16 B) ▾▾      ▾▾      ▾▾      ▾▾▾
            [XOR]   [XOR]   [XOR]   [XOR]
              │       │       │       │
              ▾       ▾       ▾       ▾
            [F2₁]   [F2₂]   [F2₃]   [F2₄]
              │       │       │       │
              ▾       ▾       ▾       ▾
           (int₁)  (int₂)  (int₃)  (int₄)


           const₁  const₂  const₃  const₄
              │       │       │       │
      int₁━━━┳┿━━━━━━┳┿━━━━━━┳┿━━━━━━┓│
             ▾▾      ▾▾      ▾▾      ▾▾
            [XOR]   [XOR]   [XOR]   [XOR]
              │       │       │       │
              ▾       ▾       ▾       ▾
            [F1₁]   [F1₂]   [F1₃]   [F1₄]
              │       │       │       │
      int₂━━━┳┿━━━━━━┳┿━━━━━━┳┿━━━━━━┓│
             ▾▾      ▾▾      ▾▾      ▾▾
            [XOR]   [XOR]   [XOR]   [XOR]
              │       │       │       │
              ▾       ▾       ▾       ▾
            [F1₁]   [F1₂]   [F1₃]   [F1₄]
              │       │       │       │
              ┊       ┊       ┊       ┊  (9 rounds skipped)
              │       │       │       │
      int₄━━━┳┿━━━━━━┳┿━━━━━━┳┿━━━━━━┓│
             ▾▾      ▾▾      ▾▾      ▾▾
            [XOR]   [XOR]   [XOR]   [XOR]
              │       │       │       │
              ▾       ▾       ▾       ▾
            [F1₁]   [F1₂]   [F1₃]   [F1₄]
              │       │       │       │
      int₁━━━┳┿━━━━━━┳┿━━━━━━┳┿━━━━━━┓│
             ▾▾      ▾▾      ▾▾      ▾▾
            [XOR]   [XOR]   [XOR]   [XOR]
              │       │       │       │
              ▾       ▾       ▾       ▾
           (out₁)  (out₂)  (out₃)  (out₄)
                  (message digest)

# Security

## Preimage Resistance

Computing preimage is presumed to be roughly equivalent to breaking 13 rounds of AES.

## Second Preimage Resistance

There are currently no known second preimage attacks easier than the generic bruteforce attack.

## Collision Resistance

There are currently no known collision attacks easier than the generic birthday attack.

Noteworthy, creating freestart collisions appears to be feasible.  No
complexity estimation for this attack is currently available.  We encourage
further research of this topic.

Also, no path was proposed to use a freestart collision to achieve a full
StreamHash5 collision.

# Performance

![StreamHash5 performance](/tests/performance.svg)
