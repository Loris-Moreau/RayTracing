# Performace Document

***All Render were Done on an MSI Katana Laptop***

---

```
//BaseBalls(Set 2 (3 Different Balls) = 1, Reflective = 1 / Transparent = 0)
case 1: BaseBalls(1, 0);
        break;
```
Time to Render : 5 min *(No Compute Shaders & No Multi Threading)*

```
case 2: Checkers();
        break;
```
Time to Render : 3 min 30 sec *(No Compute Shaders & No Multi Threading)*

```
// Amount of spheres (smol), recomended 7~11
case 3: RandomSpheres(7);
        break;
```
Time to Render : 4 min *(No Compute Shaders & No Multi Threading)*

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
Time to Render : 4 min 34 sec *(No Compute Shaders & No Multi Threading)*

```
case 6: Quads();
        break;
```
Time to Render : 6 min *(No Compute Shaders & No Multi Threading)*

```
case 7: SimpleLight();
        break;
```
Time to Render : 5 min *(No Compute Shaders & No Multi Threading)*

```
case 8: CornellBox();
        break;
```
Time to Render : 6 min 30 sec *(No Compute Shaders & No Multi Threading)*

```
case 9: CornellSmoke();
        break;
```
Time to Render : 7 min *(No Compute Shaders & No Multi Threading)*

```
case 10: FinalSceneB2(600, 200, 50, 20, 500); // <-- 
        break;
```
Time to Render : 5 Hrs *(No Compute Shaders & No Multi Threading)*

``` 
case 0: FinalSceneB2(400, 70, 30, 20, 100); 
        break;
```
Time to Render : 26 min *(No Compute Shaders & No Multi Threading)*

---


