# Performace Document

***All Render were Done on an MSI Katana Laptop (nvidia 3060 Laptop & Intel i7 11th Gen)***

---

```
//BaseBalls(Set 2 (3 Different Balls) = 1, Reflective = 1 / Transparent = 0)
case 1: BaseBalls(1, 0);
        break;
```
Time to Render : 30 sec *(No Compute Shaders & No Multi Threading)*

Time to Render : 20 sec *(Compute Shaders)*

```
case 2: Checkers();
        break;
```
Time to Render : 30 sec *(No Compute Shaders & No Multi Threading)*

Time to Render : 30 sec *(Compute Shaders)*

```
// Amount of spheres (smol), recomended 7~11
case 3: RandomSpheres(7);
        break;
```
Time to Render : 30 sec *(No Compute Shaders & No Multi Threading)*

Time to Render : 15 sec *(Compute Shaders)*

```
case 4: //Earth();
        std::clog << "no" << '\n';
        break;
```
Time to Render : N/A

```
case 5: PerlinSphere();
        break;
```
Time to Render : 30 sec *(No Compute Shaders & No Multi Threading)*

Time to Render : 15 sec *(Compute Shaders)*

```
case 6: Quads();
        break;
```
Time to Render : 1 min *(No Compute Shaders & No Multi Threading)*

Time to Render : 20 sec *(Compute Shaders)*

```
case 7: SimpleLight();
        break;
```
Time to Render : 1 min *(No Compute Shaders & No Multi Threading)*

Time to Render : 15 sec *(Compute Shaders)*

```
case 8: CornellBox();
        break;
```
Time to Render : 2min 30 sec *(No Compute Shaders & No Multi Threading)*

Time to Render : 30 sec *(Compute Shaders)*

```
case 9: CornellSmoke();
        break;
```
Time to Render : 3 min *(No Compute Shaders & No Multi Threading)*

Time to Render : 25 sec *(Compute Shaders)*

```
case 10: FinalSceneB2(600, 200, 50, 20, 500); // <-- 
        break;
```
Time to Render : 5 Hrs *(No Compute Shaders & No Multi Threading)*

Time to Render : 30 sec *(Compute Shaders)*


---

