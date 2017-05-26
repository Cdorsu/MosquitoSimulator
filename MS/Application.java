
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.util.ArrayList;

public class Application extends JFrame {

    public JButton Convert;
    public JButton Browse;
    public JButton BrowseSave;
    public JTextField FileToLoad;
    public JTextField FileToSave;
    public JCheckBox hasNormals;
    public JCheckBox CheckBoxIsRH;
    public JCheckBox MultipleObjects;
    public JCheckBox CalculateOtherVectors;
    public JCheckBox CalculateAABB;
    public JLabel Message;
    public boolean HasNormals = true;
    
    private class Texture {
        public float u;
        public float v;
        public Texture() {
            u = 0.0f;
            v = 0.0f;
        }
        public Texture(float u, float v) {
            this.u = u;
            this.v = v;
        }
        public Texture subtractFrom(Texture lhs) {
            return new Texture(lhs.u - u, lhs.v - v);
        }
    }
    
    private class Color {
        public float r;
        public float g;
        public float b;
        public Color() {
            
        }
        public Color(float r, float g, float b) {
            this.r = r;
            this.g = g;
            this.b = b;
        }
        @Override
        public String toString() {
            return r + " " + g + " " + b;
        }
    }
    
    private class Position {
        public float x;
        public float y;
        public float z;
        public Position() {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }
        public Position(float x, float y, float z) {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        
        public boolean equals(Position rhs) {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }
        
        public boolean planeEquals(Position rhs) {
            return x == rhs.x && z == rhs.z;
        }
        
        public Position subtractFrom(Position lhs) {
            return new Position(lhs.x - x,
                lhs.y - y, lhs.z - z);
        }
        
        public void Normalize() {
            float length = (float) Math.sqrt(x*x + y*y + z*z);
            x /= length;
            y /= length;
            z /= length;
        }
        
        @Override
        public String toString() {
            return x + " " + y + " " + z;
        }
    }
    
    private class SVertex {
        public SVertex() {
            
        }
        public SVertex(float x, float y, float z,
                float u, float v,
                float nx, float ny, float nz,
                boolean bHasTexture, boolean bHasNormals) {
            Pos = new Position(x,y,z);
            Tex = new Texture(u,v);
            Nor = new Position(nx,ny,nz);
            Tan = new Position();
            Bin = new Position();
            this.bHasTexture = bHasTexture;
            this.bHasNormals = bHasNormals;
            this.bHasBinormals = false;
            this.bHasTangents = false;
        }
        
        @Override
        public String toString() {
            /*return new String(Pos.x + " " + Pos.y + " " + Pos.z + 
                    " " + Tex.u + " " + Tex.v + " " + Nor.x + " " +
                    Nor.y + " " + Nor.z);*/
            String result;
            result = Pos.x + " " + Pos.y + " " + Pos.z + " ";
            if (bHasTexture)
                result += Tex.u + " " + Tex.v + " ";
            if (bHasNormals)
                result += Nor.x + " " + Nor.y + " " + Nor.z + " ";
            if (bHasTangents)
                result += Tan.x + " " + Tan.y + " " + Tan.z + " ";
            if (bHasBinormals)
                result += Bin.x + " " + Bin.y + " " + Bin.z + " ";
            return result;
        }
        
        public void SetTangent(Position Tangent) {
            Tan.x = Tangent.x;
            Tan.y = Tangent.y;
            Tan.z = Tangent.z;
            bHasTangents = true;
        }
        
        public void SetBinormal(Position Binormal) {
            Bin.x = Binormal.x;
            Bin.y = Binormal.y;
            Bin.z = Binormal.z;
            bHasBinormals = true;
        }
        
        public void SetNormals(Position Normal) {
            Nor.x = Normal.x;
            Nor.y = Normal.y;
            Nor.z = Normal.z;
            bHasNormals = true;
        }
        
        Position Pos;
        Texture Tex;
        Position Nor;
        Position Tan;
        Position Bin;
        
        boolean bHasTexture;
        boolean bHasNormals;
        boolean bHasTangents;
        boolean bHasBinormals;
    }
    
    private class Output {
        
        public String Name;
        public String MaterialToUse;
        public ArrayList Vertices;
        public ArrayList Indices;
        public Position MinAABB;
        public Position MaxAABB;
        public Output() {
            Vertices = new ArrayList<>();
            Indices = new ArrayList<>();
            MinAABB = new Position(Float.MAX_VALUE,Float.MAX_VALUE,Float.MAX_VALUE);
            MaxAABB = new Position(Float.MIN_VALUE,Float.MIN_VALUE,Float.MIN_VALUE);
        }
        public Output(String Name) {
            Vertices = new ArrayList<>();
            Indices = new ArrayList<>();
            MinAABB = new Position(Float.MAX_VALUE,Float.MAX_VALUE,Float.MAX_VALUE);
            MaxAABB = new Position(Float.MIN_VALUE,Float.MIN_VALUE,Float.MIN_VALUE);
            this.Name = Name;
        }
    }
    
    private class Material {
        public Material() {
            AmbientMap = "";
            DiffuseMap = "";
            SpecularMap = "";
            BumpMap = "";
            SpecularPower = -1;
        }
        public Material(String name) {
            Name = name;
            AmbientMap = "";
            DiffuseMap = "";
            SpecularMap = "";
            BumpMap = "";
            SpecularPower = -1;
        }
        public String Name;
        public float SpecularPower;
        public Color AmbientColor;
        public Color DiffuseColor;
        public Color EmissiveColor;
        public Color SpecularColor;
        public String AmbientMap;
        public String DiffuseMap;
        public String SpecularMap;
        public String BumpMap;
        
    }
    
    public static void main(String[] args) {
        Application app = new Application();
    }
    
    public Application() {
        this.setTitle("Apa Borsec's *.obj to *.aba converter");
        //this.setTitle("Breaking Soft's *.obj to *.aba converter");
        this.setSize(600,180);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setLocationRelativeTo(null);
        this.setResizable(false);
        
        JPanel thePanel = new JPanel();
        
        FileToLoad = new JTextField("",40);
        FileToLoad.setToolTipText("File to be converted");
        thePanel.add(FileToLoad);
        
        Browse = new JButton("Browse");
        Browse.setToolTipText("Browse computer for a *.obj file");
        myActionListener al = new myActionListener();
        Browse.addActionListener(al);
        thePanel.add(Browse);
        
        FileToSave = new JTextField("",40);
        FileToSave.setToolTipText("File to be saved");
        thePanel.add(FileToSave);
        
        BrowseSave = new JButton("Browse");
        BrowseSave.setToolTipText("Where to save the result");
        BrowseSave.addActionListener(al);
        thePanel.add(BrowseSave);
        
        myItemListener il = new myItemListener();
        
        CheckBoxIsRH = new JCheckBox("Uses Right-Hand coordinate system?");
        CheckBoxIsRH.setSelected(true);
        CheckBoxIsRH.addItemListener(il);
        thePanel.add(CheckBoxIsRH);
        
        hasNormals = new JCheckBox("Has normals");
        hasNormals.setText("Do you want to use the normals?");
        hasNormals.setSelected(true);
        hasNormals.addItemListener(il);
        thePanel.add(hasNormals);
        
        Convert = new JButton("Convert");
        Convert.setToolTipText("Convert the file");
        Convert.addActionListener(al);
        thePanel.add(Convert);
        
        MultipleObjects = new JCheckBox("Multiple Objects?");
        thePanel.add(MultipleObjects);
        
        CalculateOtherVectors = new JCheckBox("Other vectors?");
        thePanel.add(CalculateOtherVectors);
        
        CalculateAABB = new JCheckBox("Calculate AABB?");
        thePanel.add(CalculateAABB);
        
        Message = new JLabel("");
        thePanel.add(Message);
        
        this.add(thePanel);
        this.setVisible(true);
        FileToLoad.requestFocus();

    }
    
    private class myItemListener implements ItemListener {

        @Override
        public void itemStateChanged(ItemEvent e) {
            if (e.getSource() == hasNormals) {
                if (e.getStateChange() == ItemEvent.SELECTED) {
                    HasNormals = true;
                } else if (e.getStateChange() == ItemEvent.DESELECTED) {
                    HasNormals = false;
                }
            }
        }
        
    }
    
    private class myActionListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
            if (e.getSource() == Browse) {
                
                JFileChooser FileChooser = new JFileChooser();
                FileChooser.setDialogTitle("Open file ...");
                FileChooser.setAcceptAllFileFilterUsed(false);
                FileChooser.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);
                FileNameExtensionFilter filter = new FileNameExtensionFilter("Obj Files(*.obj)", "obj");
                FileChooser.setFileFilter(filter);
                int returnVal = FileChooser.showOpenDialog(null);
                
                if (returnVal == JFileChooser.APPROVE_OPTION) {
                    File file = FileChooser.getSelectedFile();
                    FileToLoad.setText(file.getPath());
                } else {
                    System.out.println("User cancelled selection");
                }
                
            } else if (e.getSource() == BrowseSave) { 
                        
                JFileChooser FileChooser = new JFileChooser();
                FileChooser.setDialogTitle("Open file ...");
                FileChooser.setAcceptAllFileFilterUsed(false);
                FileChooser.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);
                FileNameExtensionFilter filter = new FileNameExtensionFilter("Aba Borsec Asset(*.aba)", "aba");
                FileChooser.setFileFilter(filter);
                int returnVal = FileChooser.showSaveDialog(null);
                if (returnVal == JFileChooser.APPROVE_OPTION) {
                    String name = FileChooser.getSelectedFile().getPath();
                    if (!name.endsWith(".aba")) {
                        name = name + ".aba";
                    }
                    FileToSave.setText(name);
                } else {
                    System.out.println("User cancelled selection");
                }
                
            } else if (e.getSource() == Convert) {

                try {
                    File fisier = new File(FileToLoad.getText());
                    Scanner scan = new Scanner(fisier);
                    Message.setText("Opened file and started reading from file");
                    HandleInput(scan, CheckBoxIsRH.getSelectedObjects() != null);
                } catch (FileNotFoundException ex) {
                    Logger.getLogger(Application.class.getName()).log(Level.SEVERE, null, ex);
                }
                
            }
        }
    }
    void PrintResult(ArrayList Objects, ArrayList Materials) {
        Writer writer = null;
        try {
            writer = new BufferedWriter(new OutputStreamWriter(
            new FileOutputStream(FileToSave.getText()), "utf-8"));
            Message.setText("Opened file for the result; started writing the result");
            writer.write("Objects: " + Objects.size() +"\n");
            for (int i = 0; i < Objects.size(); ++i) {
                Output obj = (Output) Objects.get(i);
                Position CenterOfMass;
                float distX = (obj.MaxAABB.x - obj.MinAABB.x) / 2.0f;
                float distY = (obj.MaxAABB.y - obj.MinAABB.y) / 2.0f;
                float distZ = (obj.MaxAABB.z - obj.MinAABB.z) / 2.0f;
                CenterOfMass = new Position(obj.MaxAABB.x - distX,
                                obj.MaxAABB.y - distY, obj.MaxAABB.z - distZ);
                writer.write(obj.Name + " {\n");
                writer.write("    Vertices: " + obj.Vertices.size() + " ");
                if (((SVertex)obj.Vertices.get(0)).bHasTexture)
                    writer.write("Texture: ENABLED ");
                else
                    writer.write("Texture: DISABLED ");
                if (((SVertex)obj.Vertices.get(0)).bHasNormals)
                    writer.write("Normals: ENABLED ");
                else
                    writer.write("Normals: DISABLED ");
                if (((SVertex)obj.Vertices.get(0)).bHasTangents)
                    writer.write("Tangents: ENABLED ");
                else
                    writer.write("Tangents: DISABLED ");
                if (((SVertex)obj.Vertices.get(0)).bHasBinormals)
                    writer.write("Binormals: ENABLED");
                else
                    writer.write("Binormals: DISABLED");
                writer.write(" {\n");
                for (int j = 0; j < obj.Vertices.size(); ++j) {
                    SVertex v = (SVertex) obj.Vertices.get(j);
                    writer.write("        " + v + "\n");
                }
                writer.write("    }\n");
                writer.write("    Indices: " + obj.Indices.size() + " {\n        ");
                for (int j = 0; j < obj.Indices.size(); ++j) {
                    if (j % 3 == 0 && j > 1)
                        writer.write("\n        ");
                    writer.write((int)obj.Indices.get(j) + " ");
                }
                writer.write("\n    }\n");
                writer.write("    Material " + obj.MaterialToUse + " {\n");
                int index = -1;
                for (int j = 0; j < Materials.size(); ++j) {
                    System.out.println(obj.MaterialToUse + " == " + ((Material)Materials.get(j)).Name);
                    if (((Material)Materials.get(j)).Name.equals(obj.MaterialToUse))
                        index = j;
                }
                if (index == -1)
                    writer.write("        Couldn't find the material\n");
                else {
                    Material mat = (Material) Materials.get(index);
                    if (mat.AmbientColor != null)
                        writer.write("        Ambient color: " + mat.AmbientColor + "\n");
                    if (mat.DiffuseColor != null)
                        writer.write("        Diffuse color: " + mat.DiffuseColor + "\n");
                    if (mat.SpecularColor != null)
                        writer.write("        Specular color: " + mat.SpecularColor + "\n");
                    if (mat.SpecularPower != -1)
                        writer.write("        Specular power: " + mat.SpecularPower + "\n");
                    if (mat.EmissiveColor != null)
                        writer.write("        Emissive color: " + mat.EmissiveColor + 
                                " # probably won't use this\n");
                    if (!mat.AmbientMap.equals(""))
                        writer.write("        Ambient map: " + mat.AmbientMap + "\n");
                    if (!mat.DiffuseMap.equals(""))
                        writer.write("        Diffuse map: " + mat.DiffuseMap + "\n");
                    if (!mat.SpecularMap.equals(""))
                        writer.write("        Specular map: " + mat.SpecularMap + "\n");
                    if (!mat.BumpMap.equals(""))
                        writer.write("        Bump map: " + mat.BumpMap + "\n");
                }
                writer.write("    ");
                writer.write("}\n");
                writer.write("    Physics: {\n");
                writer.write("        Name: " + obj.Name + "\n");
                writer.write("        Weight: 0.0\n");
                if (CalculateAABB.isSelected())
                    writer.write("        AABB: MinAABB " + obj.MinAABB + " MaxAABB " + obj.MaxAABB + "\n");
                else
                    writer.write("        AABB: CALCULATED\n");
                writer.write("        Collision shape: # Box / Cone / " +
                        " Capsule / Sphere / Static plane / Cylinder / Triangle mesh\n");
                writer.write("        Motion State: Default: Quaternion: 0 0 0 1 , Transform: 0 0 0\n");
                writer.write("        Center of mass: " + CenterOfMass + "\n");
                writer.write("    }\n");
                writer.write("}\n");
            }
            
        } catch (IOException ex) {
            Logger.getLogger(Application.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            try {
                writer.close();
                Message.setText("File closed. You can see the result now");
            } catch (IOException ex) {
                
            }
        }
        
    }
    void ReadMaterialFiles(ArrayList mtlfiles, ArrayList Materials) {
        for (int i = 0; i < mtlfiles.size(); ++i) {
            String materialFilePath = new File(FileToLoad.getText()).getAbsolutePath();
            int lastIndexOf = materialFilePath.lastIndexOf("\\");
            String subSequence = (String) materialFilePath.subSequence(0, lastIndexOf + 1);
            try {
                Scanner scan = new Scanner(new File(subSequence + (String) mtlfiles.get(i)));
                String ch;
                Message.setText("Trying to read a material file(" + (String) mtlfiles.get(i) + ")");
                while (scan.hasNext()) {
                    ch = scan.next();
                    if (ch.equals("newmtl")) {
                        String matname = scan.next();
                        Materials.add(new Material(matname));
                    } else if (ch.equals("Ns")) {
                        ((Material)Materials.get(Materials.size() - 1)).
                                SpecularPower = 100.0f - (scan.nextFloat() / 100.0f);
                    } else if (ch.equals("Ka")) {
                        Color Ambient = new Color();
                        Ambient.r = scan.nextFloat();
                        Ambient.g = scan.nextFloat();
                        Ambient.b = scan.nextFloat();
                        ((Material)Materials.get(Materials.size()-1)).AmbientColor = Ambient;
                    } else if (ch.equals("Kd")) {
                        Color Diffuse = new Color();
                        Diffuse.r = scan.nextFloat();
                        Diffuse.g = scan.nextFloat();
                        Diffuse.b = scan.nextFloat();
                        ((Material)Materials.get(Materials.size()-1)).DiffuseColor = Diffuse;
                    } else if (ch.equals("Ks")) {
                        Color Specular = new Color();
                        Specular.r = scan.nextFloat();
                        Specular.g = scan.nextFloat();
                        Specular.b = scan.nextFloat();
                        ((Material)Materials.get(Materials.size()-1)).SpecularColor = Specular;
                    } else if (ch.equals("Ke")) {
                        Color Emissive = new Color();
                        Emissive.r = scan.nextFloat();
                        Emissive.g = scan.nextFloat();
                        Emissive.b = scan.nextFloat();
                        ((Material)Materials.get(Materials.size()-1)).EmissiveColor = Emissive;
                    } else if (ch.equals("map_Ka")) {
                        ((Material)Materials.get(Materials.size()-1)).AmbientMap = scan.next();
                    } else if (ch.equals("map_Kd")) {
                        ((Material)Materials.get(Materials.size()-1)).DiffuseMap = scan.next();
                    } else if (ch.equals("map_Ks")) {
                        ((Material)Materials.get(Materials.size()-1)).SpecularMap = scan.next();
                    } else if (ch.equals("map_bump") || ch.equals("bump")) {
                        ((Material)Materials.get(Materials.size()-1)).BumpMap = scan.next();
                    } else {
                        Message.setText("Found an unknown material code (" + ch + ")");
                    }
                }
            } catch (FileNotFoundException ex) {
                Logger.getLogger(Application.class.getName()).log(Level.SEVERE, null, ex);
            }
            
        }
    }
    void HandleInput(Scanner scan, boolean isRH) {
        String mtllib;
        ArrayList mtllibs = new ArrayList<>();
        boolean bHasTexture = false;
        boolean bHasNormals = false;
        ArrayList Vertices = new ArrayList<>();
        ArrayList Textures = new ArrayList<>();
        ArrayList Normals = new ArrayList<>();
        ArrayList Objects = new ArrayList<>();
        ArrayList VertexIndices = new ArrayList<>();
        ArrayList Indices = new ArrayList<>();
        ArrayList Materials = new ArrayList<>();
        int index, vertexCount;
        index = 0;
        vertexCount = 0;
        int objectCount, toSubtractV, toSubtractT, toSubtractN;
        objectCount = 0;
        toSubtractV = 0;
        toSubtractT = 0;
        toSubtractN = 0;
        while (scan.hasNext()) {
            String ch = scan.next();
            if (ch.startsWith("#")) {
                Message.setText("Found a comment; skipping the comment");
                scan.nextLine();
            } else if (ch.equals("mtllib")) {
                mtllib = scan.next();
                mtllibs.add(mtllib);
                Message.setText("Found a material library; saving it for later reading");
            } else if (ch.equals("o") || ch.equals("g")) {
                if (objectCount == 0) {
                    Message.setText("Found first object; Begin reading data about it");
                    Objects.add(new Output(scan.next()));
                    objectCount++;
                } else if (MultipleObjects.isSelected()) {
                    Message.setText("Found another object; Finishing " +
                            "loading the last object and start reading data about this object");
                    if (Textures.isEmpty())
                        Textures.add(new Texture(0.0f,0.0f));
                    if (Normals.isEmpty())
                        Normals.add(new Position(0.0f,0.0f,0.0f));
                    for (int i = 0; i < index; ++i) {
                        ((Output) Objects.get(Objects.size() - 1)).Indices.add((int) Indices.get(i));
                    }
                    for (int i = 0; i < vertexCount; ++i) {
                        int pIndex,tIndex,nIndex;
                        pIndex = (int) ((Position)VertexIndices.get(i)).x - 1;
                        tIndex = (int) ((Position)VertexIndices.get(i)).y - 1;
                        nIndex = (int) ((Position)VertexIndices.get(i)).z - 1;
                        pIndex -= toSubtractV;
                        tIndex -= toSubtractT;
                        nIndex -= toSubtractN;
                        if (!bHasTexture)
                            tIndex = 0;
                        if (!bHasNormals || !HasNormals)
                            nIndex = 0;
                        Position pos = (Position) Vertices.get(pIndex);
                        Texture tex = (Texture) Textures.get(tIndex);
                        Position nor = (Position) Normals.get(nIndex);
                        ((Output) Objects.get(Objects.size() - 1)).Vertices.add(new SVertex(
                            pos.x,pos.y,pos.z,tex.u,tex.v,nor.x,nor.y,nor.z, bHasTexture,
                                HasNormals));
                    }
                    toSubtractT += Textures.size();
                    toSubtractV += Vertices.size();
                    toSubtractN += Normals.size();
                    bHasTexture = false;
                    bHasNormals = false;
                    Objects.add(new Output(scan.next()));
                    objectCount++;
                    vertexCount = 0;
                    index = 0;
                    Vertices = new ArrayList<>();
                    Textures = new ArrayList<>();
                    Normals = new ArrayList<>();
                    VertexIndices = new ArrayList<>();
                    Indices = new ArrayList<>();
                    
                } else {
                    ch = scan.next();
                }
            } else if (ch.equals("v")) {
                Message.setText("Found a position; Reading the position ...");
                float x, y, z;
                x = scan.nextFloat();
                y = scan.nextFloat();
                z = scan.nextFloat();
                if (isRH)
                    z *= -1;
                Vertices.add(new Position(x,y,z));
                if (CalculateAABB.isSelected()) {
                    Output Obj = ((Output) Objects.get(Objects.size() - 1));
                    Obj.MinAABB.x = Math.min(Obj.MinAABB.x,x);
                    Obj.MinAABB.y = Math.min(Obj.MinAABB.y,y);
                    Obj.MinAABB.z = Math.min(Obj.MinAABB.z,z);
                    
                    Obj.MaxAABB.x = Math.max(Obj.MaxAABB.x, x);
                    Obj.MaxAABB.y = Math.max(Obj.MaxAABB.y, y);
                    Obj.MaxAABB.z = Math.max(Obj.MaxAABB.z, z);
                }
            } else if (ch.equals("vt")) {
                Message.setText("Found texture coordinates; Reading the texture coordinates ...");
                float u, v;
                u = scan.nextFloat();
                v = scan.nextFloat();
                if (isRH)
                    v = 1.0f - v;
                Textures.add(new Texture(u,v));
                bHasTexture = true;
            } else if(ch.equals("vn")) {
                Message.setText("Found a normal; Reading normal position ...");
                float x, y, z;
                x = scan.nextFloat();
                y = scan.nextFloat();
                z = scan.nextFloat();
                if (isRH)
                    z *= -1;
                Normals.add(new Position(x,y,z));
                bHasNormals = true;
            } else if (ch.equals("usemtl")) {
                ((Output)Objects.get(Objects.size() - 1)).MaterialToUse = scan.next();
            } else if (ch.equals("s")) {
                scan.nextLine();
            } else if (ch.equals("f")) {
                String Line = scan.nextLine();
                Scanner scane = new Scanner(Line);
                ArrayList IndicesForVertices = new ArrayList<>();
                while (scane.hasNext()) {
                    IndicesForVertices.add(scane.next());
                }
                int numTriangles = IndicesForVertices.size() - 2;
                if (numTriangles < 1) {
                    FileToLoad.setText("Couldn't load file");
                    break;
                }
                Message.setText("Found a new face. Start reading it");
                int veryFirstIndex = 0;
                int veryLastIndex = 0;
                for (int i = 0; i < 3; ++i) {
                    
                    String CurrentIndices = (String) IndicesForVertices.get(i);
                    int pos = 0, tex = 0, nor = 0;
                    String Current = "";
                    int whichPart = 0;
                    for (int j = 0; j < CurrentIndices.length(); ++j) {
                        if (CurrentIndices.charAt(j) == '/' || j == CurrentIndices.length() - 1) {
                            if (whichPart == 0) {
                                pos = Integer.parseInt(Current);
                                whichPart++;
                            } else if (whichPart == 1) {
                                if (!bHasTexture) {
                                    whichPart++;
                                    continue;
                                }
                                if (j == CurrentIndices.length() - 1)
                                    Current += CurrentIndices.charAt(j);
                                tex = Integer.parseInt(Current);
                                whichPart++;
                            } else if (whichPart == 2) {
                                if (!bHasNormals || !HasNormals) {
                                    continue; // Same As break;
                                }
                                if (j == CurrentIndices.length() - 1)
                                    Current += CurrentIndices.charAt(j);
                                nor = Integer.parseInt(Current);
                                whichPart++;
                            }
                            Current = "";
                        } else {
                            Current += CurrentIndices.charAt(j);
                        }
                    }
                    
                    boolean bFound = false;
                    if (VertexIndices.size() > 3) {
                        Position cPos = new Position(pos,tex,nor);
                        for (int j = 0; j < VertexIndices.size(); ++j) {
                            if (cPos.planeEquals((Position) VertexIndices.get(j))) {
                                bFound = true;
                                Indices.add(j);
                                break;
                            }
                        }
                    }
                    
                    if (!bFound) {
                        VertexIndices.add(new Position(pos,tex,nor));
                        Indices.add(VertexIndices.size() - 1);
                        vertexCount++;
                    }
                    
                    if (i == 0)
                        veryFirstIndex = (int) Indices.get(index);
                    else if (i == 2)
                        veryLastIndex = (int) Indices.get(index);
                    index++;
                }
                if (numTriangles > 1)
                    Message.setText("The face need to be retriangulated; Performing retriangulation");
                for (int i = 0; i < numTriangles - 1; ++i) {
                    Indices.add(veryFirstIndex);
                    index++;
                    
                    Indices.add(veryLastIndex);
                    index++;
                    
                    String CurrentIndices = (String) IndicesForVertices.get(i + 3);
                    int pos = 0, tex = 0, nor = 0;
                    String Current = "";
                    int whichPart = 0;
                    
                     for (int j = 0; j < CurrentIndices.length(); ++j) {
                        if (CurrentIndices.charAt(j) == '/' || j == CurrentIndices.length() - 1) {
                            if (whichPart == 0) {
                                pos = Integer.parseInt(Current);
                                whichPart++;
                            } else if (whichPart == 1) {
                                if (!bHasTexture) {
                                    whichPart++;
                                    continue;
                                }
                                if (j == CurrentIndices.length() - 1)
                                    Current += CurrentIndices.charAt(j);
                                tex = Integer.parseInt(Current);
                                whichPart++;
                            } else if (whichPart == 2) {
                                if (!bHasNormals || !HasNormals) {
                                    continue; // Same As break;
                                }
                                if (j == CurrentIndices.length() - 1)
                                    Current += CurrentIndices.charAt(j);
                                nor = Integer.parseInt(Current);
                                whichPart++;
                            }
                            Current = "";
                        } else {
                            Current += CurrentIndices.charAt(j);
                        }
                        
                    }
                    boolean bFound = false;
                    Position cPos = new Position(pos,tex,nor);
                    for (int l = 0; l < VertexIndices.size(); ++l) {
                        if (cPos.planeEquals((Position) VertexIndices.get(l))) {
                            bFound = true;
                            Indices.add(l);
                            break;
                        }
                    }
                        
                    if (!bFound) {
                        VertexIndices.add(new Position(pos,tex,nor));
                        Indices.add(VertexIndices.size() - 1);
                        vertexCount++;
                    }
                        
                    veryLastIndex = (int) Indices.get(index);
                    index++;
                     
                    
                }
            } else {
                System.out.print(ch);
                FileToLoad.setText("Couldn't load file");
                break;
            }
        }
        Message.setText("Finished loading last object. Constructing it ...");
        for (int i = 0; i < index; ++i) {
            ((Output) Objects.get(Objects.size() - 1)).Indices.add((int) Indices.get(i));
        }
        if (Textures.isEmpty())
            Textures.add(new Texture(0.0f,0.0f));
        if (Normals.isEmpty())
            Normals.add(new Position(0.0f,0.0f,0.0f));
        for (int i = 0; i < vertexCount; ++i) {
            int pIndex,tIndex,nIndex;
            pIndex = (int) ((Position)VertexIndices.get(i)).x - 1;
            tIndex = (int) ((Position)VertexIndices.get(i)).y - 1;
            nIndex = (int) ((Position)VertexIndices.get(i)).z - 1;
            pIndex -= toSubtractV;
            tIndex -= toSubtractT;
            nIndex -= toSubtractN;
            if (!bHasTexture)
                tIndex = 0;
            if (!bHasNormals || !HasNormals)
                nIndex = 0;
            Position pos = (Position) Vertices.get(pIndex);
            Texture tex = (Texture) Textures.get(tIndex);
            Position nor = (Position) Normals.get(nIndex);
            ((Output) Objects.get(Objects.size() - 1)).Vertices.add(new SVertex(
                pos.x,pos.y,pos.z,tex.u,tex.v,nor.x,nor.y,nor.z, bHasTexture, HasNormals));
        }
        if (CalculateOtherVectors.isSelected()) {
            CalculateOtherVectorsUtil(HasNormals == true ? bHasNormals != true : false,
                    ((Output) Objects.get(Objects.size() - 1)).Vertices,
                    ((Output) Objects.get(Objects.size() - 1)).Indices);
        }
        ReadMaterialFiles(mtllibs,Materials);
        PrintResult(Objects,Materials);
    }
    private void CalculateTangentAndBinormal(Position p1, Position p2, Position p3,
            Texture t1, Texture t2, Texture t3, Position Tangent, Position Binormal) {
        
        float den;
        Position edge1 = new Position();
        Position edge2 = new Position();
        Texture tex1 = new Texture();
        Texture tex2 = new Texture();
        
        edge1.x = p2.x - p1.x;
        edge1.y = p2.y - p1.y;
        edge1.z = p2.z - p1.z;
        
        edge2.x = p3.x - p1.x;
        edge2.y = p3.y - p1.y;
        edge2.z = p3.z - p1.z;
        
        tex1.u = t2.u - t1.u;
        tex1.v = t2.v - t1.v;
        
        tex2.u = t3.u - t1.u;
        tex2.v = t3.v - t1.v;
        
        den = 1.0f / (tex1.u * tex2.v - tex1.v * tex2.u);
        
        Tangent.x = (tex2.v * edge1.x - tex1.v * edge2.x) * den;
        Tangent.y = (tex2.v * edge1.y - tex1.v * edge2.y) * den;
        Tangent.z = (tex2.v * edge1.z - tex1.v * edge2.z) * den;
        Tangent.Normalize();
        
        Binormal.x = (tex1.u * edge2.x - tex2.u * edge1.x) * den;
        Binormal.y = (tex1.u * edge2.y - tex2.u * edge1.y) * den;
        Binormal.z = (tex1.u * edge2.z - tex2.u * edge1.z) * den;
        Binormal.Normalize();
        
        
    }
    public void CalculateOtherVectorsUtil(boolean CalculateNormals,
            ArrayList Vertices, ArrayList Indices) {
        for (int i = 0; i < Indices.size() / 3; ++i) {
            SVertex vertex1 = (SVertex) Vertices.get((int) Indices.get(i * 3 + 0));
            SVertex vertex2 = (SVertex) Vertices.get((int) Indices.get(i * 3 + 1));
            SVertex vertex3 = (SVertex) Vertices.get((int) Indices.get(i * 3 + 2));
            Position Tangent = new Position();
            Position Binormal = new Position();
            CalculateTangentAndBinormal(vertex1.Pos,vertex2.Pos,vertex3.Pos,
                    vertex1.Tex,vertex2.Tex,vertex3.Tex, Tangent, Binormal);
            ((SVertex) Vertices.get((int) Indices.get(i * 3 + 0))).SetTangent(Tangent);
            ((SVertex) Vertices.get((int) Indices.get(i * 3 + 0))).SetBinormal(Binormal);
            ((SVertex) Vertices.get((int) Indices.get(i * 3 + 1))).SetTangent(Tangent);
            ((SVertex) Vertices.get((int) Indices.get(i * 3 + 1))).SetBinormal(Binormal);
            ((SVertex) Vertices.get((int) Indices.get(i * 3 + 2))).SetTangent(Tangent);
            ((SVertex) Vertices.get((int) Indices.get(i * 3 + 2))).SetBinormal(Binormal);
            
            if (CalculateNormals) {
                Position Normal = new Position();
                Normal.x = Tangent.y * Binormal.z - Tangent.z * Binormal.y;
                Normal.y = Tangent.z * Binormal.x - Tangent.x * Binormal.z;
                Normal.z = Tangent.x * Binormal.y - Tangent.y * Binormal.x;
                Normal.Normalize();
                ((SVertex) Vertices.get((int) Indices.get(i * 3 + 0))).SetNormals(Normal);
                ((SVertex) Vertices.get((int) Indices.get(i * 3 + 1))).SetNormals(Normal);
                ((SVertex) Vertices.get((int) Indices.get(i * 3 + 2))).SetNormals(Normal);
            }
        }
    }
}
