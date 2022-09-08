
import { CocosParams, NativePackTool } from "../base/default";
import * as ps from 'path';
import * as fs from 'fs-extra';
import { cchelper, Paths } from "../utils";
import { randomBytes } from "crypto";
import { outputJSONSync } from 'fs-extra';


export interface IOrientation {
    landscapeLeft: boolean;
    landscapeRight: boolean;
    portrait: boolean;
    upsideDown: boolean;
}

export interface OHOSParam {
    sdkPath: string;
    ndkPath: string;
    orientation: IOrientation;
    packageName: string;
}

export class OpenHarmonyPackTool extends NativePackTool {
    params!: CocosParams<OHOSParam>;

    async create() {
        await this.copyCommonTemplate();
        await this.copyPlatformTemplate();
        await this.generateCMakeConfig();

        const ohosProjDir = this.paths.platformTemplateDirInPrj;
        const cocosXRoot = ps.normalize(Paths.nativeRoot);
        const platformParams = this.params.platformParams;
        // local.properties
        await cchelper.replaceInFile([
            { reg: '^sdk\\.dir.*', text: `sdk.dir=${platformParams.sdkPath}` },
        ], ps.join(ohosProjDir, 'local.properties'));

        // entry/build-profile.json5
        await cchelper.replaceInFile([
            { reg: '={DRES_DIR}', text: `=${cchelper.fixPath(this.paths.buildDir!)}` },
            { reg: '={DCOMMON_DIR}', text: `=${cchelper.fixPath(process.env.COMMON_DIR || '')}` },
            { reg: /"compileSdkVersion": *,/, text: `"compileSdkVersion": ${platformParams.apiLevel},}` },
        ], ps.join(ohosProjDir, 'entry/build-profile.json5'));

        const cfgFile = ps.join(ohosProjDir, 'entry/src/main/config.json');
        const configJSON = fs.readJSONSync(cfgFile);
        configJSON.app.bundleName = platformParams.packageName;
        outputJSONSync(cfgFile, configJSON, { spaces: 2 });

        const stringJSONPath = ps.join(ohosProjDir, 'entry/src/main/resources/base/element/string.json');
        const stringJSON = fs.readJSONSync(stringJSONPath);
        stringJSON.string.find((item: any) => item.name === 'MainAbility_label').value = this.params.projectName;
        outputJSONSync(stringJSONPath, stringJSON, { spaces: 2 });

        const packageJsonPath = ps.join(ohosProjDir, 'package.json');
        const packageJson = fs.readJSONSync(packageJsonPath);
        packageJson.name = this.params.projectName;
        fs.writeJSONSync(packageJsonPath, packageJson, {
            spaces: 4,
        });

        return true;
    }

    async make() {
        const ohosProjDir = this.paths.platformTemplateDirInPrj;
        if (!fs.existsSync(ps.join(ohosProjDir, 'node_modules'))) {
            console.debug(`Start install project ${ohosProjDir}`);
            // install
            await cchelper.runCmd('npm', ['i'], false, ohosProjDir);
        }
        console.debug(`Start buildOhosHap in ${ohosProjDir}`);
        // npm run buildOhosHap
        await cchelper.runCmd('npm', ['run', 'build'], false, ohosProjDir);
        return true;
    }
    // --------------- run ------------------//
    async run(): Promise<boolean> {

        return true;
    }

    private selectHap(projectDir: string, outputMode: string): string {
        const outputDir =
            ps.join(projectDir, `entry/build/outputs/hap/${outputMode}`);
        return ps.join(outputDir, this.selectHapFile(outputDir, outputMode));
    }

    private selectHapFile(outputDir: string, outputMode: string): string {
        if (!fs.existsSync(outputDir)) {
            throw new Error(`directory ${outputDir} does not exist!`);
        }
        const hapFiles = fs.readdirSync(outputDir).filter(x => x.endsWith(`.hap`));
        if (hapFiles.length === 0) {
            throw new Error(`no hap found in ${outputDir}`);
        } else if (hapFiles.length === 1) {
            return hapFiles[0];
        }
        const opt1 = hapFiles.filter(
            x => x.endsWith('-signed.hap') && x.startsWith(`entry-${outputMode}-`));
        const opt2 = hapFiles.filter(x => x.startsWith(`entry-${outputMode}`));
        return opt1.length > 0 ? opt1[0] :
            (opt2.length > 0 ? opt2[0] : hapFiles[0]);
    }

    get hdcPath(): string | null {
        if (this.params.platformParams.sdkPath) {
            return ps.join(this.params.platformParams.sdkPath, 'toolchains/hdc');
        }
        return null;
    }

    randString(n: number): string {
        if (n <= 0) {
            return '';
        }
        let rs = '';
        try {
            rs = randomBytes(Math.ceil(n / 2)).toString('hex').slice(0, n);
        } catch (ex) {
            rs = '';
            const r = n % 8;
            const q = (n - r) / 8;
            let i: number = 0;
            for (; i < q; i++) {
                rs += Math.random().toString(16).slice(2);
            }
            if (r > 0) {
                rs += Math.random().toString(16).slice(2, i);
            }
        }
        return rs;
    }
}

